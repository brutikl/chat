#include "client.h"
#include <chrono>
#include <iostream>

Client::Client(string host, int port)
{
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    inet_pton(_addr.sin_family, host.c_str(), &_addr.sin_addr.s_addr);
}

void Client::start_client()
{
    if (!connect())
        return;

    fds = {{this->_fdsock, POLLIN, 0}, {STDIN_FILENO, POLLIN, 0}};

    string msg;
    unsigned int sended = 0, received = 0;

    std::chrono::time_point<std::chrono::system_clock> start;
    start = std::chrono::system_clock::now();

    while (true) {
        int n = poll(fds.data(), fds.size(), 1000);
        if (n > 0) {
            if (fds[1].revents) {
                getline(std::cin, msg);

                if (msg == "EXIT") {
                    close(_fdsock);
                    break;
                } else if (msg == "STAT") {
                    std::chrono::duration<double> elapsed_seconds =
                            std::chrono::system_clock::now() - start;
                    cout << "Отправлено сообщений на сервер: " << sended << endl;
                    cout << "Получено сообщений от сервера: " << received << endl;
                    cout << "Время сессии: " << elapsed_seconds.count() << " sec" << endl;
                    continue;
                }

                this->send(msg + "\n", _fdsock, true);
                ++sended;
            } else if (fds[0].revents) {
                if (receive(_fdsock, MSG_DONTWAIT | MSG_PEEK) == 0) {
                    close(_fdsock);
                    break;
                }
                cout << receive(4096);
                ++received;
            }
        }
    }
}

bool Client::connect()
{
    if (::connect(_fdsock, (struct sockaddr *) &_addr, sizeof(_addr)) < 0) {
        cout << "Не удалось подключиться" << endl;
        perror("connect");
        return false;
    };
    return true;
}
