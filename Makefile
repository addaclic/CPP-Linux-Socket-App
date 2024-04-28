FLAGS = -Wall -Wextra -Werror

server: clean
	g++ $(FLAGS) server/server.cc -o server.out
	./server.out < hello.txt

clean:
	rm server.out