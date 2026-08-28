#include "request.h"
#define BUF_SIZE 500


char* get_status_code(enum status_code status_code)
{
  if(status_code      == OK) return "200 OK";
  else if(status_code == NOT_FOUND) return "404 Not Found";
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

void get_directory_files(char current_request_content[MAX_CONTENT][SITES_PATH_MAX_LEN], char* directory_path)
{
  DIR *open_directory = opendir(directory_path);
  errno = 0;
  struct dirent *directory_content;

  int i = 0;
  while((directory_content = readdir(open_directory)) != NULL)
  {
    if(strcmp(directory_content->d_name, ".") == 0 || strcmp(directory_content->d_name, "..") == 0) 
    {
      continue;
    }
    else
    {
      printf("Copying %s", directory_content->d_name);
      strcpy(current_request_content[i], directory_content->d_name);
      printf(" to %s\n", current_request_content[i]);
      i++;
    }
  }
  if(errno != 0)
  {
    perror("Error");
    exit(1);
  }
  closedir(open_directory);
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
  dprintf(
    peer_sock_fd,
    "HTTP/1.1 %s\r\n"
    "Content-Type:%s\r\n"
    "Content-Lenght:%i\r\n"
    "\r\n",
    get_status_code(status_code),
    get_content_type(path),
    BUF_SIZE 
  );
  struct stat site_stat;

  fstat(data_fd, &site_stat);

  sendfile(peer_sock_fd, data_fd, NULL, site_stat.st_size);
 
  close(data_fd);

}

void write_response(struct HttpRequest request, int peer_sock_fd, struct Config config)
{
  if(request.status_code == OK)
    {
      get_directory_files(request.request_content, request.uri);
      for(int i = 0; strlen(request.request_content[i]) > 0; i++)
      { 
        send_response(request.request_content[i], peer_sock_fd, request.status_code, request.uri);  
      } 
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
        get_status_code(request.status_code)
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
