#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define IPV4 "127.0.0.1" 
#define PORT 8080
#define BUF_SIZE 500
int main()
{
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  
  struct sockaddr_in socket_addr;
  socket_addr.sin_family = AF_INET;
  socket_addr.sin_port = htons(PORT); 
  socket_addr.sin_addr.s_addr = inet_addr(IPV4);

  if(bind(sock_fd, (struct sockaddr *)&socket_addr , sizeof(socket_addr)) == -1)
  {
    perror("Error bind");
    return -1;
  }

  if(listen(sock_fd, 2) == -1)
  {  
    perror("Error listen");
    return -1;
  }
  struct sockaddr_in peer_addr;
  socklen_t peer_addr_size = sizeof(peer_addr);
  int accepted_sock_fd;
  if((accepted_sock_fd = accept(sock_fd, (struct sockaddr *)&peer_addr, &peer_addr_size)) == -1)  
  {
    perror("Error accept");
    return -1;
  }

  char request[BUF_SIZE];
  read(accepted_sock_fd, request, sizeof(request));
  char http_header[20];
  char http_body[20];
 

  // Não tá funcionando, não consegui entender como mandar a response direito. 
  strncpy(http_header, "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=ISO-8859-4\r\nContent-Length: 400\r\n\r\n", sizeof(http_header));
  strncpy(http_body, "<h1>Hello world </h1>", sizeof(http_body));
  
  printf("%li\n", send(accepted_sock_fd, http_header, sizeof(http_header), 0));
  
  printf("%li\n", send(accepted_sock_fd, http_body, sizeof(http_body), 0));
  
  

  close(sock_fd);
  close(accepted_sock_fd);
  
  return 0;
}
