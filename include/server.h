#ifndef SERVER_H
#define SERVER_H

#include "socket.h"
#include "request.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
 
int start_server(char *config_path);

#endif //SERVER_H
