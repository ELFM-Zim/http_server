#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int start_socket(sa_family_t sin_family, int64_t port, const char *ipv4);
int accept_connection(int sock_fd);

#endif // SOCKET_H
