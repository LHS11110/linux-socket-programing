#include <iostream>
#include "../libs/TcpSocket.hpp"
using namespace std;

int main(void)
{
    TCP listener;
    listener << Address("127.0.0.1", 4040);

    TCP client;
    listener >> client;

    client.write("Hi!", 3);
    client.close_socket();
    listener.close_socket();

    return 0;
}