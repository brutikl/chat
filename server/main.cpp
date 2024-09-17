#include "server.h"
#include <chrono>
#include <iostream>
#include <map>
#include <thread>
#include <utility>
#include <vector>

using namespace std;

int main() {
  Server server("127.0.0.1", 9999);
  server.start_server();
  return 0;
}
