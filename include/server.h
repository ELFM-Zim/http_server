#ifndef SERVER_H
#define SERVER_H

#include "request.h"
#include "socket.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <pthread.h>

int start_server(char *config_path);

#endif //SERVER_H
