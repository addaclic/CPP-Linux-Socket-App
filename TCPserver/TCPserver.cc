#include "common/common.h"
#include "server.h"

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

  common::Socket server_socket;

  if (server_socket.get_fd() < 0)
    throw std::runtime_error(COLOR_RED + std::string("Error in socket") +
                             COLOR_RESET);

  server_socket.MakeNonblock();
  server_socket.structInit();

  if (server_socket.BindSocket() < 0)
    throw std::runtime_error(COLOR_RED + std::string("Error in bind") +
                             COLOR_RESET);

  if (server_socket.ListenSocket() < 0)
    throw std::runtime_error(COLOR_RED + std::string("Error in listen") +
                             COLOR_RESET);

  std::vector<int> client_fds;  // file descriptors of clients

  while (true) {
    if (input == "exit") break;

    std::vector<struct pollfd> fds;
    fds.push_back({server_socket.get_fd(), POLLIN, 0});

    for (auto fd : client_fds) {
      fds.push_back({fd, POLLIN, 0});
    }

    int ret = poll(fds.data(), fds.size(), 1000);
    if (ret < 0) {
      throw std::runtime_error(COLOR_RED + std::string("Error in poll") +
                               COLOR_RESET);
    } else if (ret == 0) {
      continue;
    }

    if (fds[0].revents & POLLIN) {
      int client_fd = accept(server_socket.get_fd(), nullptr, nullptr);

      if (client_fd < 0) {
        throw std::runtime_error(COLOR_RED + std::string("Error in accept") +
                                 COLOR_RESET);
      }

      client_fds.push_back(client_fd);
    }

    for (int i = 1; i < static_cast<int>(fds.size()); i++) {
      if (fds[i].revents & POLLIN) {
        std::cout << "reading" << std::endl;  //  need to checks
        std::string msg(1024, '\0');
        int s = recv(fds[i].fd, msg.data(), msg.size(), 0);
        LOG(COLOR_GREEN << "readed ");  //  need to check
        if (s < 0) {
          throw std::runtime_error(COLOR_RED + std::string("Error in receive") +
                                   COLOR_RESET);
        } else if (s == 0) {
          LOG(COLOR_CYAN << "Closed");  //  need to check
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

        if (command == "#exit") {
          msg = "Goodbye!";
          send(fds[i].fd, msg.c_str(), msg.size(), 0);
        } else if (command == "#count_connection") {
          msg = std::to_string(client_fds.size());
          send(fds[i].fd, msg.c_str(), msg.size(), 0);
        } else if (command == "#letter_count") {
          std::string result = parse_msg(text);
          send(fds[i].fd, result.c_str(), result.size(), 0);
        } else {
          for (auto fd : client_fds) {
            if (fd != fds[i].fd) {
              std::string new_msg =
                  COLOR_CYAN + std::string("from users: ") + COLOR_RESET + msg;
              send(fd, new_msg.c_str(), new_msg.size(), 0);
            }
          }
        }
      }
    }
  }
  exit_thread.join();
  for (auto fd : client_fds) {
    close(fd);
  }
  return 0;
}

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