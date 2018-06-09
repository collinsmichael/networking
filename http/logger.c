#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *date(void) {
    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    static char now[32];
    strftime(now, 32, "%Y/%m/%d %H:%M:%S", tm_info);
    return now;
}

int logger(char *color, char *level, char *format, ...) {
    va_list args;
    char *now = date();

    va_start(args, format);
    fprintf(stdout, "%s [%s] ", now, level);
    vfprintf(stdout, format, args);
    va_end(args);

    va_start(args, format);
    fprintf(stderr, "%s", color);
    fprintf(stderr, "%s [%s] ", now, level);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\x1B[0m");
    va_end(args);
    return 1;
}