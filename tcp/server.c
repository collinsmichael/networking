#include <arpa/inet.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "network.h"

#define role "server"

static int server;
static struct sockaddr_in saddr;
static struct sockaddr_in caddr;

int startup(int port) {
    server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server < 0) {
        fail("error creating server socket!");
    }

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);
    if (bind(server, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        fail("error port %d unavailable!", port);
    } else {
        pass("binding to port %d", port);
    }

    if (listen(server, 64) < 0) {
        fail("error listening to port %d not possible", port);
    } else {
        pass("listening to port %d", port);
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "usage: "role" port\n");
        exit(0);
    }

    int port = atoi(argv[1]);
    if (startup(port)) {
        for (;;) {
            unsigned int len = sizeof(caddr);
            int client = accept(server, (struct sockaddr*)&caddr, &len);
            if (client < 0) {
                warn("client failed to connect!");
            } else {
                info("client connected");
            }

            char echo[12];
            int size = sizeof(echo);

            memset(echo, 0, sizeof(echo));
            int took = recv(client, echo, size, 0);
            if (took != size) {
                warn("message only partially received!");
            }

            int sent = send(client, echo, size, 0);
            if (sent != size) {
                warn("message only partially sent!");
            }
        }
    }
    return 0;
}