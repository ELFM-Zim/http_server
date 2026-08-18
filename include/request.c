#include "request.h"
#define BUF_SIZE 500

void parse_request(struct HttpRequest current_request, int client_fd, struct Config *config)
{
  char *buf_request;
  if(recv(client_fd, buf_request, BUF_SIZE, 0) == -1)
  {
    perror("Error");
    exit(1);
  }
  


  strtok(buf_request, " ");
  printf("%s\n", buf_request);
}

void process_request(int peer_sock_fd, struct Config *config)
{
  struct HttpRequest request; 
  char *index = "/index.html";
  char path[strlen(config->sites.site[0].root)+strlen(index)];
  strcat(path, config->sites.site[0].root);
  strcat(path, index); 
  char recieve_buf[BUF_SIZE];

  char send_buf[BUF_SIZE];
  int data_fd;
  if((data_fd = open(path, 0, O_RDONLY)) == -1)
  {
   perror("Error");
   printf("%s\n", path);
   exit(1);
  }
  printf("%i\n", data_fd);
  parse_request(request, peer_sock_fd, config);

  dprintf(peer_sock_fd,
    "HTTP/1.1 200 OK\r\n"
    "Content-Type:text/html\r\n"
    "Content-Lenght:%li\r\n"
    "\r\n",
  strlen(send_buf)
  );
  
  struct stat site_stat;

  fstat(data_fd, &site_stat);

  sendfile(peer_sock_fd, data_fd, NULL, site_stat.st_size);

  //dprintf(peer_sock_fd,"%s\r\n", send_buf);

  close(data_fd);
}
