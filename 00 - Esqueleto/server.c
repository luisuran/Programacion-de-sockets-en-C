/* Servidor TCP.
El número de puerto se pasa como argumento*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // Para usar las funciones read, write, close
#include <sys/types.h>  // Para trabajar con llamadas al sistema. Necesaria para trabajar con los siguientes headers
#include <sys/socket.h> // Para trabajar con sockets
#include <netinet/in.h> // Constantes y estructuras necesarias para direcciones de dominio de internet

void error(const char *msg)
{
    perror(msg); // perror() imprime un mensaje de error descriptivo en stderr
    exit(1);     // exit() termina el proceso inmediatamente
}

int main(int argc, char *argv[])
/* argc -   Cantidad total de parámetros.
            En éste caso son dos: el nombre del archivo y el número de puerto
   argv -   Lista de parámetros
*/
{
    if (argc < 2)
    {
        fprintf(stderr, "Por favor ingrese un número de puerto\n");
        exit(1);
    }

    int sockfd, newsockfd, portno, n;

    struct sockaddr_in serv_addr, cli_addr; // Dirección IP
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM es tcp
    if (sockfd < 0)                           // Si el socket no fué creado
    {
        error("Error abriendo el socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr)); // bzero elimina tantos datos en memoria como el parámetro 2, en la posición del parámetro 1
    portno = atoi(argv[1]);                       // convertimos el número de puerto pasado como parámetro (string) a integer

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno); // htons hace que el número de puerto se guarde con el número más significativo primero (network byte order)

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Falló el binding");

    listen(sockfd, 5); // Como máximo 5 clientes conectados al mismo tiempo
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen); // Después de establecer la conexión se requiere un nuevo file descriptor
    if (newsockfd < 0)
        error("Error aceptando la conexión");

    close(newsockfd);
    close(sockfd);

    return 0;
}