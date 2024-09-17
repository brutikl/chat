#include "connhandler.h"

void ConnHandler::thread_handler()
{
    while (!thread_stop) {
        string msg = _sock.receive(4096);
        if (msg.find("SHUTDOWN") != string::npos) {
            close_callback();
            continue;
        }
        for (int destination : *conns_info | views::keys) {
            if (destination != _sock._fdsock)
                _sock.send(msg, destination, false);
        }
    }
}

void ConnHandler::stop_thread()
{
    thread_stop = true;
}

void ConnHandler::start_thread()
{
    _thread = std::thread(&ConnHandler::thread_handler, this);
    _thread.detach();
}
