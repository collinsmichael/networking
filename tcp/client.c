#include <arpa/inet.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "network.h"

#define role "client"

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        fprintf(stderr, "usage: "role" host port\n");
        exit(0);
    }

    char *host = argv[1];
    int port = atoi(argv[2]);
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        fail("failed to create socket!");
    }

    static struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family      = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(host);
    saddr.sin_port        = htons(port);
    if (connect(sock, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        fail("failed to connect to %s:%d", host, port);
    }

    char echo[12] = "HELLO WORLD";
    int size = sizeof(echo);

    int sent = send(sock, echo, size, 0);
    if (sent != size) {
        warn("message only partially sent!");
    }

    memset(echo, 0, sizeof(echo));
    int took = recv(sock, echo, size, 0);
    if (took != size) {
        warn("message only partially received!");
    }
    info("recieved %s", echo);

    close(sock);
    return 0;
}