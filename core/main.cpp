#include <iostream>
#include "../libs/env.hpp"
#include "../libs/TcpSocket.hpp"
using namespace std;

int stringToInteger(const char *str)
{
    if (str == NULL)
        return -1;
    int int32 = 0, idx = 0;
    while (str[idx] != '\0')
        int32 = (int32 * 10) + (str[idx++] - '0');
    return int32;
}

int main(void)
{
    ENV env;
    TCP listener;
    listener << Address(env["IP"], stringToInteger(env["PORT"]));

    TCP client;
    listener >> client;
    string str;
    str.resize(4096);
    client.write(&str[0], client.read(&str[0], str.length()));
    client.close_socket();
    listener.close_socket();

    return 0;
}