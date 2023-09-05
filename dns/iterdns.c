#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

int main() {
    struct hostent *host;
    char domain[] = "www.example.com";
    
    // Query the DNS server
    host = gethostbyname(domain);

    if (host == NULL) {
        herror("gethostbyname");
        return 1;
    }

    printf("Official name: %s\n", host->h_name);

    // Print the IP addresses associated with the domain
    for (int i = 0; host->h_addr_list[i] != NULL; i++) {
        struct in_addr addr;
        memcpy(&addr, host->h_addr_list[i], sizeof(struct in_addr));
        printf("IP Address %d: %s\n", i + 1, inet_ntoa(addr));
    }

    return 0;
}

