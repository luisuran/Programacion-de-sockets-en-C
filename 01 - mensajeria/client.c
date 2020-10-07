/*
Parámetros: 
argv[0] filename 
argv[1] server_ipaddress 
argv[2] portno
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> // Define la estructura hostent que guarda información de un host

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[255];
    if (argc < 3)
    {
        fprintf(stderr, "Indique IP del servidor y número de puerto\n");
        exit(1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM es tcp
    if (sockfd < 0)
        error("Error abriendo el socket");

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Host inválido");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); // bcopy copia n bytes de server(hostent) a serv_addr(sockaddr)
    serv_addr.sin_port = htons(portno);                                                  // host to network short

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Fallo en la conexión");

    while (1)
    {
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
            error("Error de escritura");

        bzero(buffer, 255);
        n = read(sockfd, buffer, 255);
        if (n < 0)
            error("Error de lectura");
        printf("Server: %s", buffer);

        int i = strncmp("SALIR", buffer, 3);
        if (i == 0)
            break;
    }

    close(sockfd);

    return 0;
}
