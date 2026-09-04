#include "socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define LISTEN_BACKLOG 50

int start_socket(sa_family_t sin_family, int64_t port, const char *ipv4)
{

  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  int optval = 1;
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
  {
      perror("Error setsockopt");
      return -1;
  }

  struct sockaddr_in socket_addr;
  socket_addr.sin_family = sin_family;
  socket_addr.sin_port = htons(port);
  socket_addr.sin_addr.s_addr = inet_addr(ipv4);

  if(bind(sock_fd, (struct sockaddr *)&socket_addr , sizeof(socket_addr)) == -1)
  {
      return -1;
  }

  if(listen(sock_fd, LISTEN_BACKLOG) == -1)
  {
      return -1;
  }

  return sock_fd;
}

int accept_connection(int sock_fd)
{
  struct sockaddr_in peer_addr;
  socklen_t peer_addr_len = sizeof(peer_addr);
  int accepted_sock_fd = accept(sock_fd, (struct sockaddr*)&peer_addr, &peer_addr_len);

  return accepted_sock_fd;
}
