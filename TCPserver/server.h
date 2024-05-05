#ifndef TCPSERVER_SERVER_H_
#define TCPSERVER_SERVER_H_

#include <fcntl.h>
#include <sys/poll.h>

#include <algorithm>
#include <sstream>
#include <vector>

std::string parse_msg(std::string& msg);

#endif  // TCPSERVER_SERVER_H_