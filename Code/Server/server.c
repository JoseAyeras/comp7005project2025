#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define ACK_MESSAGE "ACK"

// Global Variables
int server_socket = -1;
int received_count = 0;
int ack_sent_count = 0;
char *listen_ip = NULL;
int listen_port = -1;

// Cleanup function to close socket gracefully
void cleanup() {
    if (server_socket >= 0) {
        close(server_socket);
        printf("Server socket closed.\n");
    }
}

// Signal handler (Ctrl+C)
void handle_sigint(int sig) {
    printf("\nServer shutting down gracefully.\n");
    printf("Total messages received: %d\n", received_count);
    printf("Total ACKs sent: %d\n", ack_sent_count);
    cleanup();
    exit(0);
}

// Parse arguments
void parse_arguments(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s --listen-ip <IP> --listen-port <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--listen-ip") == 0 && i + 1 < argc) {
            listen_ip = argv[++i];
        } else if (strcmp(argv[i], "--listen-port") == 0 && i + 1 < argc) {
            listen_port = atoi(argv[++i]);
        }
    }

    if (!listen_ip || listen_port <= 0) {
        fprintf(stderr, "Error: Invalid IP or port.\n");
        exit(EXIT_FAILURE);
    }
}

// Create UDP socket
void setup_socket() {
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
}

// Bind socket
void bind_socket(struct sockaddr_in *server_addr) {
    memset(server_addr, 0, sizeof(*server_addr));
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = inet_addr(listen_ip);
    server_addr->sin_port = htons(listen_port);

    if (bind(server_socket, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0) {
        perror("Bind failed");
        cleanup();
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s:%d\n", listen_ip, listen_port);
}

// Receive a message from client
int receive_message(char *buffer, struct sockaddr_in *client_addr, socklen_t *addr_len) {
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t recv_len = recvfrom(server_socket, buffer, BUFFER_SIZE - 1, 0,
                                (struct sockaddr *)client_addr, addr_len);
    if (recv_len < 0) {
        perror("recvfrom failed");
        return -1;
    }

    buffer[recv_len] = '\0'; 
    received_count++;

    printf("Received from %s:%d - %s\n",
           inet_ntoa(client_addr->sin_addr),
           ntohs(client_addr->sin_port),
           buffer);
    return 0;
}

// Send ACK to client
void send_ack(struct sockaddr_in *client_addr, socklen_t addr_len) {
    ssize_t sent = sendto(server_socket, ACK_MESSAGE, strlen(ACK_MESSAGE), 0,
                          (struct sockaddr *)client_addr, addr_len);
    if (sent < 0) {
        perror("sendto failed");
    } else {
        ack_sent_count++;
        printf("ACK sent to %s:%d\n",
               inet_ntoa(client_addr->sin_addr),
               ntohs(client_addr->sin_port));
    }
}

// Server main loop
void server_loop() {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    while (1) {
        if (receive_message(buffer, &client_addr, &addr_len) == 0) {
            send_ack(&client_addr, addr_len);
        }
    }
}

int main(int argc, char *argv[]) {
    signal(SIGINT, handle_sigint);     
    parse_arguments(argc, argv);       
    setup_socket();                  

    struct sockaddr_in server_addr;
    bind_socket(&server_addr);        

    server_loop();                     

    cleanup();                         
    return 0;
}
