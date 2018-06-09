#include <arpa/inet.h>
#include <memory.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    fseek(stdin, 0, SEEK_END);

    int port = atoi(argv[1]);
    if (startup(port)) {
        for (;;) {
            // ----------------------------------------------------------------
            // accept connections from clients
            unsigned int len = sizeof(caddr);
            int client = accept(server, (struct sockaddr*)&caddr, &len);
            char *ip = inet_ntoa(caddr.sin_addr);
            if (client < 0) {
                warn("rx (%s) CONNECT FAILED!\n", ip);
            } else {
                info("rx (%s) CONNECT\n", ip);
            }

            // ----------------------------------------------------------------
            // receive HTTP request header
            char *map[1024];
            int count = 0;

            char request[1024];
            int size = sizeof(request);
            memset(request, 0, sizeof(request));
            int took = recv(client, request, size, 0);

            char method[1024];
            char path[1024];
            memset(method,  0, sizeof(method));
            memset(path,    0, sizeof(path));
            strcpy(path,    "root");
            sscanf(request, "%s %s", method, &path[strlen(path)]);

            if (strncmp(request, "GET", 3) == 0) {
                info("rx (%s) GET %d bytes (%s)\n", ip, took, path);
            } else if (strncmp(request, "POST", 4) == 0) {
                info("rx (%s) POST %d bytes (%s)\n", ip, took, path);
            } else if (strncmp(request, "PUT", 3) == 0) {
                info("rx (%s) PUT %d bytes (%s)\n", ip, took, path);
            } else if (strncmp(request, "DELETE", 6) == 0) {
                info("rx (%s) DELETE %d bytes (%s)\n", ip, took, path);
            } else {
                warn("rx (%s) ERROR %d bytes (%s)\n", ip, took, path);
            }

            // ---------------------------------------------------------------
            // TODO FIXME this code block is open to buffer overflows
            char *args = 0;
            for (args = path; *args; args++) {
                if (*args == '?') {
                    *args++ = 0;
                    for (count = 0; *args; ) {
                        if (count >= 1024) break;
                        for (map[count++] = args; *args; args++) {
                            if (*args == '=') {
                                *args++ = 0;
                                break;
                            }
                        }
                        for (map[count++] = args; *args; args++) {
                            if (*args == '&') {
                                *args++ = 0;
                                break;
                            }
                        }
                    }
                    for (int i = 0; i < count; i+=2) {
                        info("rx (%s) HTTP ARG (%s=%s)\n", ip, map[i], map[i+1]);
                    }
                    break;
                }
            }

            // ----------------------------------------------------------------
            // transmit HTTP response header
            char status[64];
            char today[64];
            char content[64];
            int length = 0;

            FILE *file = fopen(path, "rb");
            if (file) {
                strcpy(path,    "root/200.html");
                strcpy(status,  "200 OK");
                strcpy(today,   date());
                strcpy(content, "text/html; charset=UTF-8");
            } else {
                strcpy(path,    "root/404.html");
                strcpy(status,  "404 NOT FOUND");
                strcpy(today,   date());
                strcpy(content, "text/html; charset=UTF-8");
                file = fopen("root/404.html", "rb");
            }
            fseek(file, 0, SEEK_END);
            length = ftell(file);
            fseek(file, 0, SEEK_SET);

            char header[512];
            sprintf(header,
                    "HTTP/1.1 %s\n"
                    "Date: %s\n"
                    "Server: servername\n"
                    "Content-Type: %s\n"
                    "Content-Length: %d\n"
                    "\n",
                    status, today, content, length);

            send(client, header, strlen(header), 0);
            info("tx (%s) HTTP HEADER\n", ip);
            info("tx (%s)\n%s", ip, header);

            if (file) {
                while (length > 0) {
                    char response[1024];
                    int partition = (length < sizeof(response))
                        ? length
                        : sizeof(response);
                    fread(response, 1, partition, file);
                    send(client, response, partition, 0);
                    length -= partition;
                }
                info("tx (%s) HTML (%s)\n", ip, path);
            }
            close(client);
        }
    }
    return 0;
}