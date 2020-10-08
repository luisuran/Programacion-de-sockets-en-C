#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h> // Para usar isspace()

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

    char buffer[512];
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
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Fallo en la conexión");

    bzero(buffer, 512);

    FILE *f; // f es un puntero a un archivo
    int palabras = 0;

    char c;

    f = fopen("prueba.txt", "r"); // Al leer el puntero de archivo se situa al inicio de la primer palabra

    while ((c = getc(f)) != EOF) // Contamos la cantidad de palabras en el archivo.
                                 // getc(archivo) lee el siguiente caracter del archivo
    {
        fscanf(f, "%s", buffer);
        if (isspace(c) || c == '\t' || c == '\n') // Contamos las palabras
            palabras++;
    }

    write(sockfd, &palabras, sizeof(int));
    rewind(f); // Volvemos el puntero a la primera posición del archivo

    char ch;
    while (ch != EOF)
    {
        fscanf(f, "%s", buffer);
        write(sockfd, buffer, 512);
        ch = fgetc(f);
    }

    printf("El archivo se envió correctamente.\n");

    close(sockfd);

    return 0;
}
