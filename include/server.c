#include "server.h"

int start_server(char *config_path)
{
  struct Config config;
  config = parse_config(config_path);

  int sock_fd = start_socket(AF_INET, config.server.port, config.server.ipv4);
  if(sock_fd == -1)
  {
    perror("Error");
    return EXIT_FAILURE;
  }

  int accepted_sock_fd = accept_connection(sock_fd);
  if(accepted_sock_fd == -1)
  {
    perror("Error");
    close(sock_fd);
    return EXIT_FAILURE;
  }

  process_request(accepted_sock_fd, &config);
  close(accepted_sock_fd);
  close(sock_fd);
}
