#include "request.h"
#include "utils.h"
#include "config.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

void send_response_header(int peer_sock_fd, char* status_code, char* content_type, int content_lenght)
{
    dprintf(
      peer_sock_fd,
      "HTTP/1.1 %s\r\n"
      "Content-Type:%s\r\n"
      "Content-Lenght:%i\r\n"
      "\r\n",
      status_code,
      content_type,
      content_lenght
    );
}

void send_response(char* current_content_path, int peer_sock_fd, enum status_code status_code,char* request_uri)
{
  char path[strlen(request_uri)+strlen("/")+strlen(current_content_path)]; 
  memset(path, '\0', sizeof(path));
  strcat(path, request_uri);
  strcat(path, "/");
  strcat(path, current_content_path); 

  int data_fd;
  if((data_fd = open(path, 0, O_RDONLY)) == -1)
  {
   perror("Error");
   printf("Erro ao enviar o arquivo: %s\n", path);
   exit(1);
  } 

  printf("Content-Type: %s\n", get_content_type(path));
  send_response_header(peer_sock_fd, get_status_code(status_code), get_content_type(path), BUF_SIZE);
  
  struct stat site_stat;

  fstat(data_fd, &site_stat);

  sendfile(peer_sock_fd, data_fd, NULL, site_stat.st_size);
 
  close(data_fd);

}

void read_request(struct HttpRequest *current_request, int peer_sock_fd, struct Config *config)
{
  char buf_request[BUF_SIZE];
  if(recv(peer_sock_fd, buf_request, sizeof(buf_request), 0) == -1)

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
  
  current_request->status_code = NOT_FOUND;
  for(int i = 0; i < config->sites.sites_size; i++)
  {
    if(strcmp(current_request->host, config->sites.site[i].host) == 0)
    {
      strcpy(current_request->uri, config->sites.site[i].root);
      current_request->status_code = OK;
      return;
    }
  }

  //GET /foo.com HTTP/1.1 
  //
  // /foo
  // foo
}



void write_response(struct HttpRequest request, int peer_sock_fd, struct Config config)
{
  if(request.status_code == OK)
    {
      request.request_content = get_directory_files(request.uri);
      for(int i = 0; strlen(request.request_content[i]) > 0; i++)
      { 
        send_response(request.request_content[i], peer_sock_fd, request.status_code, request.uri);  
      } 
    }
    else
    {
      send_response_header(peer_sock_fd, get_status_code(request.status_code), "text/plain", 9);
    }
}


void *process_request(void* args)
{
  struct ConnectionData *connection_data = args;
  struct HttpRequest current_request;
  read_request(&current_request, connection_data->peer_sock_fd, &connection_data->config);
  write_response(current_request, connection_data->peer_sock_fd, connection_data->config); 
  close(connection_data->peer_sock_fd);
  return NULL;
}  
