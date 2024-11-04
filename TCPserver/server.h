#ifndef TCPSERVER_SERVER_H_
#define TCPSERVER_SERVER_H_

#include <sys/poll.h>

#include <algorithm>
#include <vector>

std::string parse_msg(std::string& msg);

#endif  // TCPSERVER_SERVER_H_