#include "server.h"

#include "common/common.h"

int main() {
  common::Socket listenFd;  // file descriptor

  struct sockaddr_in addr;
  // char message[1024];

  if (listenFd.get_fd() < 0) {
    throw std::runtime_error("socket");
  }
  std::cout << "Socket created" << std::endl;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(3425);                    // port 3425
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);  // 127.0.0.1 or localhost
  if (bind(listenFd.get_fd(), (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    throw std::runtime_error("bind");
  }
  std::cout << "Socket binded" << std::endl;
  if (listen(listenFd.get_fd(), 5) < 0) {  // listen with 5 max connections
    throw std::runtime_error("listen");
  }
  std::cout << "Socket listening" << std::endl;

  std::thread t([]() {
    std::string str;
    std::cin >> str;
    if (str == "exit") exit(0);
  });

  std::vector<std::shared_ptr<common::Socket>> sockets;

  while (true) {
    std::vector<struct pollfd> polls(sockets.size() + 1);
    polls.front().fd = listenFd.get_fd();
    polls.front().events = POLLIN;

    for (size_t i = 1; i < sockets.size() + 1; ++i) {
      polls[i].fd = sockets[i - 1]->get_fd();
      polls[i].events = POLLIN;
    }

    std::cerr << "Polling" << std::endl;
    int pollStatus = poll(polls.data(), polls.size(), 5'000);
    std::cerr << "Polling done" << std::endl;

    if (pollStatus < 0) {
      throw std::runtime_error("poll");
    }
    if (pollStatus == 0) {
      continue;
    }
    if (polls.front().revents & POLLIN) {
      std::shared_ptr<common::Socket> accepted_socket(
          std::make_shared<common::Socket>(
              accept(listenFd.get_fd(), NULL, NULL)));  // accept
      if (accepted_socket->get_fd() < 0) {
        throw std::runtime_error("accept");
      }
      sockets.push_back(accepted_socket);
      std::cerr << "Connected" << std::endl;
      continue;
    }

    std::vector<std::shared_ptr<common::Socket>> active_connections;
    // 00000000
    // 00000001
    // 00000011
    for (size_t i = 1; i < polls.size(); ++i) {
      std::cerr << "Polling for " << i << std::endl;
      std::cerr << "revents: " << polls[i].revents << std::endl;
      std::cout << std::endl;
      if (polls[i].revents & POLLIN) {
        std::cerr << "Receiving" << std::endl;
        std::string message(1024, '\0');
        int respLen = recv(polls[i].fd, message.data(), 1024, 0);
        std::cerr << "respLen: " << respLen << std::endl;
        if (respLen < 0) {
          continue;
        }

        // process message

        std::cout << std::string(message, respLen) << std::endl;

        active_connections.push_back(std::move(sockets[i - 1]));

        // return 1;
      }
    }

    sockets = std::move(active_connections);

    // close(accepted_socket);  remake it
  }

  t.join();
  return 0;
}