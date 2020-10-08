#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

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
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    int num1, num2, respuesta, operador;

S:
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255); // Leemos string desde el servidor
    if (n < 0)
        error("Error leyendo del socket");
    printf("Server - %s\n", buffer);
    scanf("%d", &num1);                // Ingresamos el primer número
    write(sockfd, &num1, sizeof(int)); // Enviamos el primer número al servidor

    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
        error("Error leyendo del socket");
    printf("Server - %s\n", buffer);
    scanf("%d", &num2);
    write(sockfd, &num2, sizeof(int));

    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
        error("Error leyendo del socket");
    printf("Server - %s\n", buffer);
    scanf("%d", &operador);
    write(sockfd, &operador, sizeof(int));

    if (operador == 5)
        goto Q;

    read(sockfd, &respuesta, sizeof(int));
    printf("Server - La respuesta es %d\n", respuesta);

    if (operador != 5)
        goto S;

Q:
    printf("Hasta la vista...");

    close(sockfd);

    return 0;
}
