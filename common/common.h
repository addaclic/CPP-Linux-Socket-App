#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <thread>

namespace common {
class Socket {
 public:
  Socket();

  void makeNonblock();

  void structInit();

  void structInit(char* ip, char* port);

  int connectSocket();

  int bindSocket();

  int listenSocket();

  int get_fd();

  ~Socket();

 private:
  int fd_;
  struct sockaddr_in addr_;
};
};  // namespace common

#endif  // COMMON_COMMON_H_
