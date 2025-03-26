#include <stdio.h>
#include <stdlib.h> //needed for float params
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>

// Create UDP socket
void setup_socket(int * socket) {
    if ((socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
}

// Bind socket
void bind_socket(struct sockaddr_in *server_addr, int& socket char* listen_ip, int listen_port) {
    memset(server_addr, 0, sizeof(*server_addr));
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = inet_addr(listen_ip);
    server_addr->sin_port = htons(listen_port);

    if (bind(socket, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0) {
        fprintf(stderr, "FATAL ERROR: Bind to %s:%d failed.", listen_ip, listen_port);
        cleanup();
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s:%d\n", listen_ip, listen_port);
}
