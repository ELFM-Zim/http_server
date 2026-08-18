build: 
		gcc main.c include/utils.c include/request.c include/config.c include/socket.c  -ltomlc17 -g -o servidor
