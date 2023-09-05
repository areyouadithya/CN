#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating client socket");
        exit(1);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
    server_addr.sin_port = htons(8080); // Server port

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        exit(1);
    }

    printf("Connected to server\n");

    // Send an HTTP request (simplified for demonstration)
    const char *request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    send(client_socket, request, strlen(request), 0);
    printf("Sent HTTP request\n");

    // Receive and print the server's response
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("Server response:\n%s\n", buffer);
    }

    // Close the client socket
    close(client_socket);

    return 0;
}

