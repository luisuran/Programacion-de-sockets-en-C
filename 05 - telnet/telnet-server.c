/*
 * Servidor telnet TCP
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define MYPORT 3490

#define BACKLOG 10     // cantidad de conexiones en la cola

void sigchld_handler(int s){
    while (wait (NULL) > 0);
}

int main (void) {
    int sockfd, new_fd;
    struct sockaddr_in my_addr, their_addr;
    int sin_size;
    struct sigaction sa;
    int yes = 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    my_addr.sin_family = AF_INET;           // host byte order
    my_addr.sin_port = htons(MYPORT);       // network byte order (short)
    my_addr.sin_addr.s_addr = INADDR_ANY;   // completo con mi IP
    memset (&(my_addr.sin_zero), '\0', 8);  // completo con ceros el resto del struct

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler;    // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    while (1) {     // main accept loop()
        sin_size = sizeof(struct sockaddr_in);
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            exit(1);
        }

        printf("Server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));

        if (!fork()) {      // proceso hijo
            close(sockfd);  // el proceso hijo no necesita al listener
            if (send(new_fd,  "Hola P11\n", 9, 0) == -1)
                perror("send");
            close(new_fd);
            exit(0);
        }
        close(new_fd);
    }

    return 0;
}
