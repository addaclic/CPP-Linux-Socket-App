FLAGS = -Wall -Wextra -Werror

server: TCPserver/TCPserver.cc common/common.cc
	g++ $(FLAGS) -I . TCPserver/TCPserver.cc common/common.cc -o server.out
	./server.out

client: TCPclient/TCPclient.cc common/common.cc
	g++ $(FLAGS) -I . TCPclient/TCPclient.cc common/common.cc -o client.out
	./client.out localhost 3425

clean:
	rm -rf server.out client.out