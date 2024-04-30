#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

int main(int argc, char *argv[]) {
  // we need 2 things: ip address and port number, in that order
  if (argc != 3) {
    throw std::invalid_argument(
        "Usage: need input arguments: ip_address & port");
  }
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // file descriptor

  struct sockaddr_in addr;

  if (sockfd < 0) {
    throw std::runtime_error("Error in socket");
  }
  std::cout << "Socket created" << std::endl;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(std::stoi(argv[2]));
  inet_pton(AF_INET, argv[1], &(addr.sin_addr));
  if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    throw std::runtime_error("Error in connect");
  }
  std::cout << "Socket connected" << std::endl;
  std::string massage = "Hello there!";
  int s = send(sockfd, massage.c_str(), massage.size(), 0);
  std::cout << s << std::endl;

  close(sockfd);
  return 0;
}