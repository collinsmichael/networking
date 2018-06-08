#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#define role "client"
#define pass(fmt, ...) printf(" \x1B[32m[pass] "role" "fmt"\x1B[0m", __VA_ARGS__)
#define info(fmt, ...) printf(" \x1B[36m[info] "role" "fmt"\x1B[0m", __VA_ARGS__)
#define warn(fmt, ...) printf(" \x1B[33m[warn] "role" "fmt"\x1B[0m", __VA_ARGS__)
#define fail(fmt, ...) printf(" \x1B[31m[fail] "role" "fmt"\x1B[0m", __VA_ARGS__)

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