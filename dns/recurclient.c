#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 256

const char* SERVER_IPS[] = {"127.0.0.1", "127.0.0.2", "127.0.0.3"};
const int SERVER_PORTS[] = {8080, 8081, 8082};
const int NUM_SERVERS = sizeof(SERVER_IPS) / sizeof(SERVER_IPS[0]);

int main() {
    char buffer[BUFFER_SIZE];
    const char *domain;
    int found = 0;

    for (int i = 0; i < 1; i++) {
        int client_socket;
        struct sockaddr_in server_addr;

        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1) {
            perror("Socket creation failed");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORTS[i]);
        server_addr.sin_addr.s_addr = inet_addr(SERVER_IPS[i]);


        if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            perror("Connection failed");
            continue; 
        }


        printf("Enter domain to lookup IP: ");
        scanf("%s", buffer);


        send(client_socket, buffer, strlen(buffer), 0);

        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("Servers  response: %s\n", buffer);

        if (strcmp(buffer, "Not Found") != 0) {
            found = 1;
            break;
        }

        close(client_socket);
    }

    if (!found) {
        printf("Domain not found in any server.\n");
    }

    return 0;
}
