#ifndef REQUEST_H
#define REQUEST_H

#include "config.h"
#include "http.h"
#include <linux/limits.h>
struct HttpRequest
{
  enum HTTP_METHODS method;
  char host[PATH_MAX];
  char uri[PATH_MAX];
  enum status_code status_code;
  char *request_content[PATH_MAX];
};

struct ConnectionData
{
  int peer_sock_fd;
  struct Config config;
};

void *process_request(void *args);

#endif // REQUEST_H
