#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    throw std::invalid_argument(
        "Usage: need input arguments: ip_address & port");
  }
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // file descriptor
  if (sockfd < 0) {
    throw std::runtime_error("Error in socket");
  }
  //   fcntl(sockfd, F_SETFL, O_NONBLOCK);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(std::stoi(argv[2]));
  inet_pton(AF_INET, argv[1], &(addr.sin_addr));
  if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    throw std::runtime_error("Error in connect");
  }
  std::string message = "Hello there!";
  int s = send(sockfd, message.c_str(), message.size(), 0);
  //   s = send(sockfd, message.c_str(), message.size(), 0);
  //   s = send(sockfd, message.c_str(), sizeof(message.c_str()), 0);

  std::cout << s << std::endl;
  close(sockfd);
  return 0;
}