#include "client.h"
#include "common/common.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    LOG(COLOR_RED << "Usage: " << argv[0] << " <ip> <port>");
    return 1;
  }
  common::Socket client_socket;
  if (client_socket.get_fd() < 0) {
    LOG(COLOR_RED << "Error in socket");
    return 1;
  }
  client_socket.structInit(argv[1], argv[2]);

  LOG(COLOR_YELLOW << "Trying to connect...");
  if (client_socket.ConnectSocket() < 0) {
    LOG(COLOR_RED << "Error in connect");
    return 1;
  }
  LOG(COLOR_GREEN << "Connected");

  std::thread receive_thread(recieve_msg, client_socket.get_fd());

  std::string message;
  while (true) {
    std::cin >> std::ws;
    std::getline(std::cin, message);
    int condition =
        send(client_socket.get_fd(), message.c_str(), message.size(), 0);
    if (condition <= 0) {
      LOG(COLOR_RED << "Error in send");
      break;
    }
    if (message == "#exit") {
      break;
    }
  }
  close(client_socket.get_fd());
  receive_thread.join();
  return 0;
}

void recieve_msg(int sockfd) {
  while (true) {
    std::string msg(1024, '\0');
    int s = recv(sockfd, msg.data(), msg.size(), 0);
    if (s <= 0) break;

    LOG(COLOR_CYAN << msg);
  }
};