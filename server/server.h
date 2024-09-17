#ifndef SERVER_H
#define SERVER_H
#include "../socket/socket.h"
#include "connhandler.h"
#include <iostream>
#include <map>
#include <poll.h>
#include <thread>
#include <vector>

class Server : public Socket {
public:
  Server(std::string addr, int port);

  ~Server();
  void start_server();
  void stop_server(int fd);

private:
  void bind(std::string &host, int port);
  Socket accept();
  int efd;
  map<int, ConnHandler *> connections;
  std::vector<pollfd> fds;
};

#endif // SERVER_H
