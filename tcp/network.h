#ifndef NETWORK_H
#define NETWORK_H

// ---------------------------------------------------------------------
// logging macros (stdout is redirected to ./network.log on commandline)
#define pass(fmt, ...) printf(" \x1B[32m[pass] "role" "fmt"\x1B[0m", __VA_ARGS__)
#define info(fmt, ...) printf(" \x1B[36m[info] "role" "fmt"\x1B[0m", __VA_ARGS__)
#define warn(fmt, ...) printf(" \x1B[33m[warn] "role" "fmt"\x1B[0m", __VA_ARGS__)
#define fail(fmt, ...) printf(" \x1B[31m[fail] "role" "fmt"\x1B[0m", __VA_ARGS__)

#endif // NETWORK_H