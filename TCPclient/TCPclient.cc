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
      throw;
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
  int connection_attempts = 0;
  int max_connection_attempts = 3;
  std::thread *aboba = nullptr;
  while (connection_attempts < max_connection_attempts) {
    ++connection_attempts;
    std::cout << "Trying to connect..." << std::endl;
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      std::cout << "Error in connect" << std::endl;
      continue;
    }
    std::cout << "Connected" << std::endl;
    connection_attempts = 0;

    aboba = new std::thread(recieve_msg, sockfd);

    std::string message;
    while (true) {
      std::cin >> std::ws;
      std::getline(std::cin, message);
      if (message == "#exit") {
        connection_attempts = max_connection_attempts;
        break;
      }
      int condition = send(sockfd, message.c_str(), message.size(), 0);
      std::cout << "Sended; " << condition << std::endl;
      if (condition <= 0) {
        std::cout << "Error in send" << std::endl;
        break;
      }
    }
  }
  close(sockfd);
  aboba->join();
  delete aboba;
  return 0;
}