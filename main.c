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
	socklen_t peer_addr_len = sizeof(peer_addr);
	int accepted_sock_fd = accept(sock_fd, (struct sockaddr*)&peer_addr, &peer_addr_len);
	
	char receive_buf[BUF_SIZE];
	char write_buf[BUF_SIZE];

	recv(accepted_sock_fd, receive_buf, sizeof(receive_buf), 0);
	printf("%s\n", receive_buf);
	
	dprintf(accepted_sock_fd, 
		"HTTP/1.1 200 OK\r\n"
		"Content-Type:text/html\r\n"
		"\r\n"
	);

	dprintf(accepted_sock_fd,
		"<h1> Hello world <h1>"
	);

  close(sock_fd);
	close(accepted_sock_fd);
  
  return 0;
}
