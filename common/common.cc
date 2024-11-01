#include "common.h"

common::Socket::Socket() { fd_ = socket(AF_INET, SOCK_STREAM, 0); };
common::Socket::~Socket() { close(fd_); };
void common::Socket::MakeNonblock() { fcntl(fd_, F_SETFL, O_NONBLOCK); };
int common::Socket::get_fd() { return fd_; };
void common::Socket::structInit() {
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(3425);                    // default port
  addr_.sin_addr.s_addr = htonl(INADDR_LOOPBACK);  // localhost
};
int common::Socket::BindSocket() {
  return bind(fd_, (struct sockaddr*)&addr_, sizeof(addr_));
};

int common::Socket::ListenSocket() { return listen(fd_, 5); };

void common::Socket::structInit(char* ip, char* port) {
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(std::stoi(port));
  if (ip == "localhost") ip = "127.0.0.1";
  inet_pton(AF_INET, ip, &(addr_.sin_addr));
};

int common::Socket::ConnectSocket() {
  return connect(fd_, (struct sockaddr*)&addr_, sizeof(addr_));
};