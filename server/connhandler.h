#ifndef CONNHANDLER_H
#define CONNHANDLER_H
#include "../socket/socket.h"
#include <functional>
#include <iostream>
#include <map>
#include <ranges>
#include <thread>

class ConnHandler
{
  public:
    ConnHandler(Socket _sock,
                map<int, ConnHandler *> *conns_info,
                std::function<void()> callback)
    {
        this->_sock = _sock;
        this->conns_info = conns_info;
        this->close_callback = callback;
    };

    ConnHandler() = default;
    void thread_handler();
    void stop_thread();
    void start_thread();
    ~ConnHandler()
    {
        stop_thread();
        close(_sock._fdsock);
    }

  private:
    thread _thread;
    Socket _sock;
    map<int, ConnHandler *> *conns_info = nullptr;
    std::function<void()> close_callback;

    bool thread_stop = false;
};

#endif // CONNHANDLER_H
