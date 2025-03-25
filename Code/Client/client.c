#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024
#define ACK_STRING "ACK"

int client_socket = -1;
int packets_sent = 0;
int retransmissions = 0;
int acks_received = 0;
int timeout_seconds = 3;

void cleanup() {
    if (client_socket >= 0) {
        close(client_socket);
        printf("Client socket closed.\n");
    }
}

void handle_sigint(int sig) {
    printf("\nClient shutting down gracefully.\n");
    printf("Total packets sent: %d\n", packets_sent);
    printf("Total retransmissions: %d\n", retransmissions);
    printf("Total ACKs received: %d\n", acks_received);
    cleanup();
    exit(0);
}

int wait_for_ack(struct sockaddr_in *server_addr, socklen_t addr_len) {
    char buffer[BUFFER_SIZE];
    struct timeval timeout = { timeout_seconds, 0 };
    fd_set read_fds;

    FD_ZERO(&read_fds);
    FD_SET(client_socket, &read_fds);

    if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt failed");
        return 0;
    }

    ssize_t recv_len = recvfrom(client_socket, buffer, BUFFER_SIZE - 1, 0,
                                (struct sockaddr *)server_addr, &addr_len);

    if (recv_len < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("Timeout! Retransmitting...\n");
        } else {
            perror("recvfrom error");
        }
        return 0;
    }

    buffer[recv_len] = '\0';

    // Trim newline or carriage return characters
    for (int i = 0; i < recv_len; i++) {
        if (buffer[i] == '\r' || buffer[i] == '\n') {
            buffer[i] = '\0';
            break;
        }
    }

    if (strcmp(buffer, ACK_STRING) == 0) {
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        fprintf(stderr, "Usage: %s --target-ip <IP> --target-port <PORT> --timeout <SECONDS>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *target_ip = NULL;
    int target_port = -1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--target-ip") == 0 && i + 1 < argc) {
            target_ip = argv[++i];
        } else if (strcmp(argv[i], "--target-port") == 0 && i + 1 < argc) {
            target_port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--timeout") == 0 && i + 1 < argc) {
            timeout_seconds = atoi(argv[++i]);
        }
    }

    if (!target_ip || target_port <= 0 || timeout_seconds <= 0) {
        fprintf(stderr, "Invalid arguments.\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handle_sigint);

    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(target_port);
    server_addr.sin_addr.s_addr = inet_addr(target_ip);

    printf("Client ready. Type your message and hit ENTER (Ctrl+C to quit):\n");

    char message[BUFFER_SIZE];
    while (1) {
        printf("> ");
        fflush(stdout);

        if (!fgets(message, BUFFER_SIZE, stdin)) break;

        size_t msg_len = strlen(message);
        if (message[msg_len - 1] == '\n') message[msg_len - 1] = '\0';

        int acked = 0;
        int retries = 0;

        while (!acked && retries < 5) {
            ssize_t sent = sendto(client_socket, message, strlen(message), 0,
                                  (struct sockaddr *)&server_addr, addr_len);
            if (sent < 0) {
                perror("sendto failed");
                break;
            }

            packets_sent++;
            if (wait_for_ack(&server_addr, addr_len)) {
                printf("ACK received!\n");
                acks_received++;
                acked = 1;
            } else {
                retransmissions++;
                retries++;
            }
        }

        if (!acked) {
            printf("Failed to receive ACK after retries.\n");
        }
    }

    handle_sigint(0);
    return 0;
}
