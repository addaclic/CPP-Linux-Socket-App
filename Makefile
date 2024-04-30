FLAGS = -Wall -Wextra -Werror

server: server/server.cc 
	g++ $(FLAGS) -I . server/server.cc -o server.out
	./server.out

client: client/client.cc
	g++ $(FLAGS) -I . client/client.cc -o client.out
	./client.out localhost 3425

clean:
	rm -rf server.out client.out