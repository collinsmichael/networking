#include <arpa/inet.h>
#include <memory.h>
#include <signal.h>
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

void sigkill(int code) {
    close(server);
    exit(code);
}

int startup(int port) {
    server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server < 0) {
        fail("error creating server socket!\n");
    }

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);
    if (bind(server, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        fail("error port %d unavailable!\n", port);
    } else {
        pass("binding to port %d\n", port);
    }

    if (listen(server, 64) < 0) {
        fail("error listening to port %d not possible\n", port);
    } else {
        pass("listening to port %d\n", port);
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "usage: "role" port\n");
        exit(0);
    }
    signal(SIGTERM, sigkill);
    signal(SIGKILL, sigkill);
    signal(SIGQUIT, sigkill);

    int port = atoi(argv[1]);
    if (startup(port)) {
        for (;;) {
            unsigned int len = sizeof(caddr);
            int client = accept(server, (struct sockaddr*)&caddr, &len);
            if (client < 0) {
                warn("client failed to connect!\n");
            } else {
                info("client connected\n");
            }

            char echo[1024];
            int size = sizeof(echo);

            memset(echo, 0, sizeof(echo));
            int took = recv(client, echo, size, 0);
            if (took != size) {
                warn("message only partially received!\n");
            }
            info("received %d bytes!\n%s\n",took, echo);
            fflush(stdout);

            int sent = send(client, echo, size, 0);
            if (sent != size) {
                warn("message only partially sent!\n");
            }

            close(client);
        }
    }
    return 0;
}