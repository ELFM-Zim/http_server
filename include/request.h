#ifndef REQUEST_H
#define REQUEST_H

#include "config.h"
#include "utils.h"
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <stdio.h>
#include <stdlib.h>
struct HttpRequest 
{
  enum HTTP_METHODS method;  
  char URI[SITES_PATH_MAX_LEN]; 
  char host[SITES_PATH_MAX_LEN];
};


void parse_request(struct HttpRequest current_request, int client_fd, struct Config *config);
void process_request(int sock_fd, struct Config *config);

#endif //REQUEST_H
