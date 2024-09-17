#include "server.h"

Server::Server(string addr, int port) {
  int opt = 1;
  setsockopt(this->_fdsock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  bind(addr, port);
  ::listen(_fdsock, 2);
  this->efd = eventfd(0, 0);
  fds = {{this->efd, POLLIN, 0}, {_fdsock, POLLIN, 0}};
}
Server::~Server() {
  for (auto _to_close = fds.begin(); _to_close != fds.end();) {
    if (connections.count(_to_close->fd)) {
      delete connections[_to_close->fd];
      connections.erase(_to_close->fd);
    } else
      close(_to_close->fd);
    ++_to_close;
  }
  close(_fdsock);
  close(efd);
}

void Server::start_server() {
  cout << "Сервер запущен" << endl;

  Socket client;
  while (true) {
    const int TIMEOUT = 1000;
    int n = poll(fds.data(), fds.size(), TIMEOUT);
    if (n > 0) {
      if (fds[0].revents) {
        break;
      } else if (fds[1].revents) {
        client = accept();
        if (connections.size() >= 2) {
          this->send("Достигнут максимум подключений", client._fdsock, false);
          close(client._fdsock);
          continue;
        }
        cout << "Новое подключение" << endl;

        fds.push_back(pollfd{client._fdsock, POLLIN, 0});
        connections[client._fdsock] = new ConnHandler(
            client, &connections, [&]() { stop_server(fds[0].fd); });
        connections[client._fdsock]->start_thread();

        if (connections.size() < 2) {
          this->send("Ожидаем подключение другого клиента...\n", client._fdsock,
                     true);
        } else if (connections.size() == 2) {
          for (int _sock : connections | views::keys) {
            this->send("Соединение установлено, можно начинать общение\n",
                       _sock, true);
          }
        }
        fds[1].revents = 0;
      }
    }

    bool is_disconnected = false;
    for (auto _sock = fds.begin() + 2; _sock != fds.end();) {
      if (_sock->revents and receive(_sock->fd, MSG_DONTWAIT | MSG_PEEK) == 0) {
        delete connections[_sock->fd];
        connections.erase(_sock->fd);
        _sock = fds.erase(_sock);
        is_disconnected = true;
      } else
        _sock++;
    }

    if (is_disconnected and !connections.empty()) {
      this->send("Собеседник отключился, ожидаем нового клиента...\n",
                 connections.begin()->first, true);
    }
  }

  for (auto _to_close = fds.begin(); _to_close != fds.end();) {
    if (connections.count(_to_close->fd) > 0) {
      delete connections[_to_close->fd];
      connections.erase(_to_close->fd);
    } else
      close(_to_close->fd);
    ++_to_close;
  }

  cout << "Работа завершена" << endl;
}

void Server::stop_server(int fd) {
  uint64_t stop = 1;
  write(fd, &stop, sizeof(stop));
}

void Server::bind(std::string &host, int port) {
  inet_pton(_addr.sin_family, host.c_str(), &_addr.sin_addr.s_addr);
  _addr.sin_port = htons(port);
  if (::bind(_fdsock, (sockaddr *)&_addr, sizeof(sockaddr)) < 0) {
    std::cout << "Failed to bind socket! " << strerror(errno) << "\n";
  };
}

Socket Server::accept() {
  sockaddr_in client_addr;
  uint32_t size = sizeof(client_addr);
  int client_socket = ::accept(_fdsock, (sockaddr *)&client_addr, &size);
  return Socket(client_socket, client_addr);
}
