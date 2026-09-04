build: 
		gcc main.c src/utils.c src/server.c src/http.c src/request.c src/config.c src/socket.c  -ltomlc17 -Iinclude -g -o servidor
