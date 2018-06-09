#ifndef NETWORK_H
#define NETWORK_H

// ---------------------------------------------------------------------
// logging macros (stdout is redirected to ./network.log on commandline)

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[36m"
#define RET "\x1B[0m"

static inline char *datetime(void) {
    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    static char now[32];
    strftime(now, 32, "%Y/%m/%d %H:%M:%S", tm_info);
    return now;
}

#define pass(fmt, ...) fprintf(stderr, GRN "%s [pass] "role" " fmt RET "\n", datetime(), ##__VA_ARGS__)
#define info(fmt, ...) fprintf(stderr, BLU "%s [info] "role" " fmt RET "\n", datetime(), ##__VA_ARGS__)
#define warn(fmt, ...) fprintf(stderr, YEL "%s [warn] "role" " fmt RET "\n", datetime(), ##__VA_ARGS__)
#define fail(fmt, ...) do { \
    fprintf(stderr, RED "%s [fail] "role" " fmt RET "\n", datetime(), ##__VA_ARGS__); \
    exit(-1); \
} while (0)

#endif // NETWORK_H