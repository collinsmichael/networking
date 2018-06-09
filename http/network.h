#ifndef NETWORK_H
#define NETWORK_H

// ---------------------------------------------------------------------
// logging macros (stdout is redirected to ./network.log on commandline)
char *date(void);
int logger(char *color, char *level, char *format, ...);
#define pass(fmt, ...) logger("\x1B[32m", "pass", fmt, ##__VA_ARGS__)
#define info(fmt, ...) logger("\x1B[36m", "info", fmt, ##__VA_ARGS__)
#define warn(fmt, ...) logger("\x1B[33m", "warn", fmt, ##__VA_ARGS__)
#define fail(fmt, ...) do { \
	logger("\x1B[31m", "fail", fmt, ##__VA_ARGS__);\
	exit(-1);\
} while (0)

#endif // NETWORK_H