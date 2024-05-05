FLAGS = -Wall -Wextra -Werror

server: TCPserver/TCPserver.cc 
	g++ $(FLAGS) -I . TCPserver/TCPserver.cc -o server.out
	./server.out

client: TCPclient/TCPclient.cc
	g++ $(FLAGS) -I . TCPclient/TCPclient.cc -o client.out
	./client.out localhost 3425

clean:
	rm -rf server.out client.out