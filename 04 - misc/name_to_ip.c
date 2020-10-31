/*
 Este programa devuelve la dirección ip de un cierto dominio
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    struct hostent *h;

    if (argc != 2) {
        fprintf(stderr, "Ingresar un dominio\n");
        exit(1);
    }

    if ((h = gethostbyname(argv[1])) == NULL) {
        herror("gethostbyname");
        exit(1);
    }

    printf("Nombre de dominio: %s\n", h->h_name);
    printf("Dirección IP: %s\n", inet_ntoa(*((struct in_addr *)h->h_addr)));

    return 0;
}
