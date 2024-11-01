#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <thread>

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"

#define LOG(x) std::cout << x << COLOR_RESET << std::endl

namespace common {
class Socket {
 public:
  Socket();
  ~Socket();

  void MakeNonblock();

  void structInit();

  void structInit(char* ip, char* port);

  int ConnectSocket();

  int BindSocket();

  int ListenSocket();

  int get_fd();

 private:
  int fd_;
  struct sockaddr_in addr_;
};
};  // namespace common

#endif  // COMMON_COMMON_H_
