#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
    char buffer[255];

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

    int num1, num2, respuesta, operador;

S:
    n = write(newsockfd, "Ingrese un número: ", strlen("Ingrese un número: ")); // Requerir primer número
    if (n < 0)
        error("Error de escritura...");
    read(newsockfd, &num1, sizeof(int)); // Leer primer número
    printf("El primer número es %d\n", num1);

    n = write(newsockfd, "Ingrese otro número: ", strlen("Ingrese otro número: "));
    if (n < 0)
        error("Error de escritura...");
    read(newsockfd, &num2, sizeof(int));
    printf("El segundo número es %d\n", num2);

    n = write(newsockfd, "Ingrese una operacion: \n\t1 - Suma \n\t2 - Resta \n\t3 - Multiplicacion \n\t4 - División \n\t5 - Salir",
              strlen("Ingrese una operacion: \n\t1 - Suma \n\t2 - Resta \n\t3 - Multiplicacion \n\t4 - División \n\t5 - Salir"));
    if (n < 0)
        error("Error de escritura...");
    read(newsockfd, &operador, sizeof(int));
    printf("La operación seleccionada fué %d\n", operador);

    switch (operador)
    {
    case 1:
        respuesta = num1 + num2;
        break;
    case 2:
        respuesta = num1 - num2;
        break;
    case 3:
        respuesta = num1 * num2;
        break;
    case 4:
        respuesta = num1 / num2;
        break;
    case 5:
        goto Q; // Instrucción de salto a la etiqueta Q
        break;
    }

    write(newsockfd, &respuesta, sizeof(int));
    if (operador != 5)
        goto S;

Q:
    close(newsockfd);
    close(sockfd);

    return 0;
}