#include "socket.h"
#include <ctime>
#include <iostream>

Socket::Socket()
{
    _fdsock = socket(AF_INET, SOCK_STREAM, 0);
    if (_fdsock < 0) {
        perror("socket");
        exit(1);
    }
    _addr.sin_family = AF_INET;
}

Socket::Socket(int socket, sockaddr_in info)
{
    _fdsock = socket;
    _addr = info;
}

void Socket::send(string msg, int destination, bool with_time)
{
    if (!with_time) {
        ::send(destination, msg.c_str(), msg.size(), 0);
        return;
    }

    string now_time = get_time();
    msg = now_time + msg;
    ::send(destination, msg.c_str(), msg.size(), 0);
}

string Socket::receive(int size)
{
    char buf[size];
    int bytes_read = recv(_fdsock, buf, size - 1, 0);
    buf[bytes_read] = 0;

    return string(buf);
}

string Socket::get_time()
{
    time_t now = time(0);
    tm *time = localtime(&now);
    string hour = time->tm_hour < 10 ? "0" + to_string(time->tm_hour) :
                                       to_string(time->tm_hour);
    string min =
            time->tm_min < 10 ? "0" + to_string(time->tm_min) : to_string(time->tm_min);

    string now_time = hour + ":" + min + " ";
    return now_time;
}

int Socket::receive(int fd, int flags)
{
    char check;
    return recv(fd, &check, 1, flags);
}
