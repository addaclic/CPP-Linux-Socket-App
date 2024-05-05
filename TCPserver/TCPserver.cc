#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

std::string parse_msg(std::string& msg) {
  std::stringstream ss;
  std::cout << msg << std::endl;
  char c = msg[0];
  int count = 0;
  for (size_t i = 0; i < msg.size(); ++i) {
    if (msg[i] == c) {
      ++count;
    } else {
      ss << c << ": " << count << std::endl;
      count = 1;
      c = msg[i];
    }
    if (i == msg.size() - 1) {
      ss << c << ": " << count << std::endl;
    }
  }

  return ss.str();
}

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

  std::vector<int> client_fds;  // file descriptors of clients

  while (true) {
    if (input == "exit") {
      break;
    }
    std::vector<struct pollfd> fds;
    fds.push_back({server_fd, POLLIN, 0});
    for (auto fd : client_fds) {
      fds.push_back({fd, POLLIN, 0});
    }
    int ret = poll(fds.data(), fds.size(), 1000);
    if (ret < 0) {
      throw std::runtime_error("Error in poll");
    } else if (ret == 0) {
      std::cout << "No activity in 1 second" << std::endl;  //  timeout
      continue;
    }

    if (fds[0].revents & POLLIN) {
      int client_fd = accept(server_fd, nullptr, nullptr);
      if (client_fd < 0) {
        throw std::runtime_error("Error in accept");
      }
      client_fds.push_back(client_fd);
    }
    for (int i = 1; i < static_cast<int>(fds.size()); i++) {
      if (fds[i].revents & POLLIN) {
        std::cout << "reading" << std::endl;  //  need to checks
        std::string msg(1024, '\0');
        int s = recv(fds[i].fd, msg.data(), msg.size(), 0);
        std::cout << "readed " << s << std::endl;  //  need to check
        if (s < 0) {
          throw std::runtime_error("Error in recv");
        } else if (s == 0) {
          std::cout << "Closed" << std::endl;  //  need to check
          close(fds[i].fd);
          auto pos =
              std::find_if(client_fds.begin(), client_fds.end(),
                           [&fds, i](int fd) { return fd == fds[i].fd; });
          client_fds.erase(pos);
          continue;
        }
        msg.resize(s);
        std::string command, text;
        if (!msg.empty() && msg[0] == '#') {
          auto first_space = msg.find_first_of(' ');
          if (first_space != std::string::npos) {
            command = msg.substr(0, first_space);
            try {
              text = msg.substr(first_space + 1);
            } catch (std::out_of_range&) {
              //
            }
          } else {
            command = msg;
          }
        }
        std::cout << "command: " << command << ", text: " << text << std::endl;
        if (command == "#count_connection") {
          msg = std::to_string(client_fds.size());
          send(fds[i].fd, msg.c_str(), msg.size(), 0);
        } else if (command == "#letter_count") {
          std::string result = parse_msg(text);
          send(fds[i].fd, result.c_str(), result.size(), 0);
        } else {
          for (auto fd : client_fds) {
            if (fd != fds[i].fd) send(fd, msg.c_str(), msg.size(), 0);
          }
        }
      }
    }
  }
  exit_thread.join();
  for (auto fd : client_fds) {
    close(fd);
  }
  close(server_fd);
  return 0;
}