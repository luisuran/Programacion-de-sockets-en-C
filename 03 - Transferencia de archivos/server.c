#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Por favor ingrese un número de puerto\n");
        exit(1);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[512];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error abriendo el socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Falló el binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
        error("Error aceptando la conexión");

    FILE *fp;

    int ch = 0;
    fp = fopen("prueba_recibido.txt", "a"); // Si no existe el fichero, lo crea.
                                            // Si el fichero existe agrega ("a") los datos al final del fichero
    int palabras;

    read(newsockfd, &palabras, sizeof(int));

    while (ch != palabras) // Recorre el loop 'palabras' número de veces
    {
        read(newsockfd, buffer, 512);
        fprintf(fp, "%s ", buffer);
        ch++;
    }

    printf("Archivo recibido exitosamente.\n");

    close(newsockfd);
    close(sockfd);

    return 0;
}