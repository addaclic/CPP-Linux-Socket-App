#include "client.h"
#include "common/common.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
    return 1;
  }
  common::Socket ClientSock;
  if (ClientSock.get_fd() < 0) {
    std::cout << "Error in socket" << std::endl;
    return 1;
  }
  ClientSock.structInit(argv[1], argv[2]);

  std::cout << "Trying to connect..." << std::endl;
  if (ClientSock.connectSocket() < 0) {
    std::cout << "Error in connect" << std::endl;
    return 1;
  }
  std::cout << "Connected" << std::endl;

  std::thread rcv_thread(recieve_msg, ClientSock.get_fd());

  std::string message;
  while (true) {
    std::cin >> std::ws;
    std::getline(std::cin, message);
    int condition =
        send(ClientSock.get_fd(), message.c_str(), message.size(), 0);
    if (condition <= 0) {
      std::cout << "Error in send" << std::endl;
      break;
    }
    if (message == "#exit") {
      break;
    }
  }
  close(ClientSock.get_fd());
  rcv_thread.join();
  return 0;
}

void recieve_msg(int sockfd) {
  while (true) {
    std::string msg(1024, '\0');
    int s = recv(sockfd, msg.data(), msg.size(), 0);
    if (s <= 0) break;

    std::cout << msg << std::endl;
  }
};