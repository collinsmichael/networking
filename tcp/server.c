#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "network.h"

#define role "server"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "usage: "role" port\n");
        exit(0);
    }

    int port = atoi(argv[1]);
    pass("port = %d\n", port);
    info("port = %d\n", port);
    warn("port = %d\n", port);
    fail("port = %d\n", port);
    return 0;
}