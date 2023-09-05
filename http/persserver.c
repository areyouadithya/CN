#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating server socket");
        exit(1);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080); // Use port 8080

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding server socket");
        close(server_socket);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Error listening for connections");
        close(server_socket);
        exit(1);
    }

    printf("Server listening on port 8080...\n");

    while (1) {
        // Accept incoming connections
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }

        printf("Client connected\n");

        // Handle HTTP request (simplified for demonstration)
        const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
        send(client_socket, response, strlen(response), 0);

        // Close the client socket
        close(client_socket);

        printf("Client disconnected\n");
    }

    // Close the server socket
    close(server_socket);

    return 0;
}

