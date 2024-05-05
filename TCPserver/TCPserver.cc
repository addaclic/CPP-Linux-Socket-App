#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

int main() {
  std::string input;
  std::thread exit_thread(
      [](std::string& input) {
        while (true) {
          std::cin >> input;
          if (input == "exit") {
            break;
          }
        }
      },
      std::ref(input));
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);  // file descriptor
  fcntl(server_fd, F_SETFL, O_NONBLOCK);
  if (server_fd < 0) {
    throw std::runtime_error("Error in socket");
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(3425);                    // port
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);  // 127.0.0.1
  if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    throw std::runtime_error("Error in bind");
  }
  if (listen(server_fd, 5) < 0) {
    throw std::runtime_error("Error in listen");
  }

  std::vector<int> client_fds;
  while (true) {
    if (input == "exit") {
      break;
    }
    std::cout << "Waiting for client" << std::endl;
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(server_fd, &readset);
    for (auto fd : client_fds) {
      FD_SET(fd, &readset);
    }

    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    int max_fd = server_fd;
    if (!client_fds.empty()) {
      int max_fd = *std::max_element(client_fds.begin(), client_fds.end());
      max_fd = std::max(server_fd, max_fd);
    }

    int res = select(max_fd + 1, &readset, NULL, NULL, &timeout);
    if (res < 0) {
      throw std::runtime_error("Error in select");
    }
    if (FD_ISSET(server_fd, &readset)) {
      int client_fd = accept(server_fd, NULL, NULL);
      if (client_fd < 0) {
        throw std::runtime_error("Error in accept");
      }
      client_fds.push_back(client_fd);
      std::cout << "Client accepted" << std::endl;
      //   continue;
    }

    std::cout << "Reading from clients" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // for (auto fd : client_fds) {
    //   if (FD_ISSET(fd, &readset)) {
    char buffer[1024] = {0};
    int s = read(client_fds.back(), buffer, 1024);
    if (s < 0)
      throw std::runtime_error("Error in read");
    else
      std::cout << buffer << std::endl;
    //   } else {
    //     std::cout << "No messages" << std::endl;
    //   }
    // }
  }
  exit_thread.join();
  for (auto fd : client_fds) {
    close(fd);
  }
  close(server_fd);
  return 0;
}