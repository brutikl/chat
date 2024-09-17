#ifndef CLIENT_H
#define CLIENT_H
#include "../socket/socket.h"
#include <poll.h>
#include <vector>

class Client : public Socket
{
  public:
    Client(string host, int port);
    void start_client();
    bool connect();

  private:
    std::vector<pollfd> fds;
};

#endif // CLIENT_H
