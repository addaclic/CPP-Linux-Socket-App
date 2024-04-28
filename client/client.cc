#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // file descriptor

  struct sockaddr_in addr;

  if (sockfd < 0) {
    throw std::runtime_error("Error in socket");
  }
  std::cout << "Socket created" << std::endl;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(3425);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    throw std::runtime_error("Error in connect");
  }
  std::cout << "Socket connected" << std::endl;
  std::string massage = "Hello there!";
  send(sockfd, massage.c_str(), sizeof(massage), 0);

  close(sockfd);
  return 0;
}