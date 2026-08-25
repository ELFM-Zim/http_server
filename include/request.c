#include "request.h"
#define BUF_SIZE 500


char* get_status_code(struct HttpRequest current_request)
{
  if(current_request.status_code      == OK) return "200 OK";
  else if(current_request.status_code == NOT_FOUND) return "404 Not Found";
}

char* get_content_type(char* path)
{
  
  struct content_type
  {
    char* extension;
    char* MIME;
  };
  printf("PATH: %s\n", path);
  struct content_type ct_list[] = {
    {".html" , "text/html"},
    {".htm"  , "text/html"},
    {".txt"  , "text/plain"},
    {".css"  , "text/css"},
    {".js"   , "text/javascript"},
    {".json" , "application/json"},
    {".xml"  , "application/xml"}
  };

  int ct_len = sizeof(ct_list) / sizeof(ct_list[0]);

  char* extension = strrchr(path + 1, '.');
  printf("EXTENSION: %s\n", extension);
  for(int i = 0; i < ct_len; i++)
  {
      printf("Extension: %s comparada com: %s\n", extension, ct_list[i].extension);
      if(strcmp(extension, ct_list[i].extension) == 0)
      { 
        return ct_list[i].MIME;
      }
  }
  
  return "application/octet-stream";

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
      char *index = "/index.html";
      char path[strlen(request.uri)+strlen(index)]; 
      memset(path, '\0', sizeof(path));
      strcat(path, request.uri);
      strcat(path, index); 

      char recieve_buf[BUF_SIZE];

      int data_fd;
      if((data_fd = open(path, 0, O_RDONLY)) == -1)
      {
       perror("Error");
       printf("%s\n", path);
       exit(1);
      }

      printf("Content-Type: %s\n", get_content_type(path));
      dprintf(
        peer_sock_fd,
        "HTTP/1.1 %s\r\n"
        "Content-Type:%s\r\n"
        "Content-Lenght:%i\r\n"
        "\r\n",
        get_status_code(request),
        get_content_type(path),
        BUF_SIZE 
      );
      struct stat site_stat;

      fstat(data_fd, &site_stat);

      sendfile(peer_sock_fd, data_fd, NULL, site_stat.st_size);
     
      close(data_fd);
    }
    else
    {
      dprintf(
        peer_sock_fd,
        "HTTP/1.1 %s\r\n"
        "Content-Type:text/plain\r\n"
        "Content-Length: 9\r\n"
        "\r\n"
        "Not Found",
        get_status_code(request)
      );
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
