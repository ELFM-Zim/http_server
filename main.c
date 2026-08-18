#include "include/config.h"
#include "include/socket.h"
#include "include/request.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    return -1;
  }
  struct Config config;

  config = parse_config(argv[1]);
  

  int sock_fd = start_socket(AF_INET, config.server.port, config.server.ipv4); 
  if(sock_fd == -1)
  {
    perror("Error");
    exit(1);
  }

  int accepted_sock_fd = accept_connection(sock_fd);
  if(accepted_sock_fd == -1)
  {
    perror("Error");
    exit(1);
  }

  process_request(accepted_sock_fd, &config);

  close(accepted_sock_fd);
	close(sock_fd);

  return 0;
}




