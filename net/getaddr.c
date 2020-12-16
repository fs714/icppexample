#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int lookup_host(const char *host, char *addresses) {
    struct addrinfo hints, *res;
    int errcode;
    char addrstr[100];
    void *ptr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;

    errcode = getaddrinfo(host, NULL, &hints, &res);
    if (errcode != 0) {
        return -1;
    }

    while (res) {
        inet_ntop(res->ai_family, res->ai_addr->sa_data, addrstr, 100);

        switch (res->ai_family) {
            case AF_INET:
                ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
                break;
            case AF_INET6:
                ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
                break;
        }
        inet_ntop(res->ai_family, ptr, addresses, 100);
//        printf("IPv%d address: %s (%s)\n", res->ai_family == PF_INET6 ? 6 : 4,
//               addrstr, res->ai_canonname);
        break;
//        res = res->ai_next;
    }

    return 0;
}

int main(int argc, char **argv) {
    char addresses[100];
    int errcode;
    errcode = lookup_host(argv[1], addresses);
//    errcode = lookup_host("www.baidu.com", addresses);
    if (errcode != 0) {
        exit(1);
    }

    printf("--- %s\n", addresses);
    printf("--- %lu\n", sizeof(addresses));
}
