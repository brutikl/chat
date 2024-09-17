#include "client.h"
#include <iostream>

using namespace std;

int main()
{
    Client client("127.0.0.1", 9999);
    client.start_client();
    return 0;
}
