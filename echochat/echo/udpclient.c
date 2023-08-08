#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create a socket
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change this to the server's IP address

    while (1) {
        printf("Enter a message: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Send the message to the server
        sendto(client_fd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        // Receive and display the server's response
        ssize_t bytes_received = recvfrom(client_fd, buffer, sizeof(buffer), 0, NULL, NULL);
        printf("Server response: %.*s", (int)bytes_received, buffer);
    }

    close(client_fd);
    return 0;
}

