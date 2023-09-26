#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_DOMAINS 100
#define BUFFER_SIZE 256

struct DomainIP {
    char domain[BUFFER_SIZE];
    char ip[BUFFER_SIZE];
};

struct DomainIP domainIPs[MAX_DOMAINS];
int numDomains = 0;

// Function to add domain-to-IP mappings to the array
void addDomainIP(const char *domain, const char *ip) {
    if (numDomains < MAX_DOMAINS) {
        strcpy(domainIPs[numDomains].domain, domain);
        strcpy(domainIPs[numDomains].ip, ip);
        numDomains++;
    }
}

const char* findIP(const char *domain) {
    for (int i = 0; i < numDomains; i++) {
        if (strcmp(domain, domainIPs[i].domain) == 0) {
            return domainIPs[i].ip;
        }
    }
    return "Not Found";
}

int main() {
    // Initialize some domain-to-IP mappings for server1
    addDomainIP("example.com", "192.168.1.1");
    addDomainIP("google.com", "172.217.12.206");

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Initialize server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);  // Port number for server1
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(1);
    }

    // Listen for connections
    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        exit(1);
    }

    printf("Server 1 listening on port 8080...\n");

    while (1) {
        socklen_t addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);

        // Receive domain request from client
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("Server 1 received request: %s\n", buffer);

        // Find the IP for the requested domain
        const char *ip = findIP(buffer);

        if (strcmp(ip, "Not Found") == 0) {
            // Request to the next server (server2)
            int server2_socket = socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in server2_addr;

            server2_addr.sin_family = AF_INET;
            server2_addr.sin_port = htons(8081);  // Port number for server2
            server2_addr.sin_addr.s_addr = inet_addr("127.0.0.2");

            if (connect(server2_socket, (struct sockaddr *)&server2_addr, sizeof(server2_addr)) == -1) {
                perror("Connection to server2 failed");
            } else {
                // Forward the request to server2
                send(server2_socket, buffer, strlen(buffer), 0);

                // Receive the response from server2
                recv(server2_socket, buffer, BUFFER_SIZE, 0);

                // Forward the response to the client
                send(client_socket, buffer, strlen(buffer), 0);
                close(server2_socket);
            }
        } else {
            // Send the IP back to the client
            send(client_socket, ip, strlen(ip), 0);
        }

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
