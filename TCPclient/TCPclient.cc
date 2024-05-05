#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <optional>
#include <thread>

void recieve_msg(int sockfd) {
  while (true) {
    std::string msg(1024, '\0');
    int s = recv(sockfd, msg.data(), msg.size(), 0);
    if (s <= 0) {
      // std::cout << "Error in recv" << std::endl;
      break;
    }
    std::cout << msg << std::endl;
  }
};

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
    return 1;
  }
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // file descriptor
  if (sockfd < 0) {
    std::cout << "Error in socket" << std::endl;
    return 1;
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(std::stoi(argv[2]));
  inet_pton(AF_INET, argv[1], &(addr.sin_addr));

  std::cout << "Trying to connect..." << std::endl;
  if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    std::cout << "Error in connect" << std::endl;
  }
  std::cout << "Connected" << std::endl;

  std::thread aboba(recieve_msg, sockfd);

  std::string message;
  while (true) {
    std::cin >> std::ws;
    std::getline(std::cin, message);
    int condition = send(sockfd, message.c_str(), message.size(), 0);
    if (condition <= 0) {
      std::cout << "Error in send" << std::endl;
      break;
    }
    if (message == "#exit") {
      break;
    }
  }
  close(sockfd);
  aboba.join();
  return 0;
}