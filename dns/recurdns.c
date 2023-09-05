#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

int main() {
    struct addrinfo hints, *result, *rp;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;  // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // Specify socket type
    
    // Domain name to resolve
    const char *domain = "www.example.com";
    
    int ret = getaddrinfo(domain, NULL, &hints, &result);
    if (ret != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        return 1;
    }
    
    // Iterate through the results and print IP addresses
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        void *addr;
        char ipstr[INET6_ADDRSTRLEN];
        
        if (rp->ai_family == AF_INET) {  // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {  // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)rp->ai_addr;
            addr = &(ipv6->sin6_addr);
        }
        
        // Convert the IP address to a readable string
        inet_ntop(rp->ai_family, addr, ipstr, sizeof(ipstr));
        printf("IP address: %s\n", ipstr);
    }
    
    freeaddrinfo(result);
    
    return 0;
}

