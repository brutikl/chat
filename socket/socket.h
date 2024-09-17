#ifndef SOCKET_H
#define SOCKET_H
#include <arpa/inet.h>
#include <cstdlib>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

class Socket
{
  public:
    Socket();
    Socket(int socket, sockaddr_in info);
    void send(string msg, int destination, bool with_time);
    string receive(int size);
    string get_time();
    int receive(int fd, int flags);

    int _fdsock = -1;
    sockaddr_in _addr;
};

#endif // SOCKET_H
