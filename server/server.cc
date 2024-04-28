#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // file descriptor

  struct sockaddr_in addr;
  char message[1024];

  if (sockfd < 0) {
    throw std::runtime_error("socket");
  }
  std::cout << "Socket created" << std::endl;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(3425);                    // port 3425
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);  // 127.0.0.1
  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    throw std::runtime_error("bind");
  }
  std::cout << "Socket binded" << std::endl;
  if (listen(sockfd, 5) < 0) {
    throw std::runtime_error("listen");
  }
  std::cout << "Socket listening" << std::endl;
  while (true) {
    int sock = accept(sockfd, NULL, NULL);
    if (sock < 0) {
      throw std::runtime_error("accept");
    }
    // std::cout << "Client connected" << std::endl;

    int len = recv(sock, &message, sizeof(message), 0);
    if (len < 0) {
      throw std::runtime_error("recv");
    }
    message[len] = '\0';
    std::cout << message << std::endl;

    close(sock);
  }

  close(sockfd);
  return 0;
}