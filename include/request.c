#include "request.h"
#define BUF_SIZE 500



void parse_request(struct HttpRequest *current_request, int client_fd, struct Config *config)
{
  char buf_request[BUF_SIZE];
  if(recv(client_fd, buf_request, sizeof(buf_request), 0) == -1)
  {
    perror("Error");
    exit(1);
  }
  
  char method_string[BUF_SIZE];
  strcpy(method_string, strtok(buf_request, " "));

  char request_path[BUF_SIZE];
  strcpy(request_path, strtok(NULL, " "));
  
  printf("method string: %s\n", method_string);
  printf("request path: %s\n", request_path);
  
  current_request->method = parse_http_method(method_string);
  strcpy(current_request->host, request_path+1); //remove /
  
  for(int i = 0; i < config->sites.sites_size; i++)
  {
    if(strcmp(current_request->host, config->sites.site[i].host) == 0)
    {
      strcpy(current_request->uri, config->sites.site[i].root);
    }
  }

  //GET /foo.com HTTP/1.1 
  //
  // /foo
  // foo
}


void process_request(int peer_sock_fd, struct Config *config)
{
  struct HttpRequest request; 
  char *index = "/index.html";
  
  parse_request(&request, peer_sock_fd, config);

  char path[strlen(request.uri)+strlen(index)];
  strcat(path, request.uri);
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
