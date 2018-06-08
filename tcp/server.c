#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include "network.h"

#define role "server"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "usage: "role" port\n");
        exit(0);
    }

    int port = atoi(argv[1]);
    pass("port = %d", port);
    info("port = %d", port);
    warn("port = %d", port);
    fail("port = %d", port);
    return 0;
}