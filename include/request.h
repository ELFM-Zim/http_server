#ifndef REQUEST_H
#define REQUEST_H

#include "config.h"
#include "http.h"
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>

#define MAX_CONTENT 16

enum status_code {OK, NOT_FOUND};

struct HttpRequest 
{
  enum HTTP_METHODS method;  
  char host[SITES_PATH_MAX_LEN];
  char uri[SITES_PATH_MAX_LEN];
  enum status_code status_code;
  char request_content[MAX_CONTENT][SITES_PATH_MAX_LEN];
};

struct ConnectionData
{
  int peer_sock_fd;
  struct Config config;
};

void *process_request(void* args);
#endif //REQUEST_H
