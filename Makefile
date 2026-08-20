build: 
		gcc main.c include/server.c include/http.c include/request.c include/config.c include/socket.c  -ltomlc17 -g -o servidor
