#include <iostream>
#include <unordered_map>
#include <functional>
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

class match_making
{
public:
    unordered_map<string, function<void(vector<string>)>> map;
    unordered_map<string, string> info;

public:
    void operator()(TCP& listener)
    {
        
    }
};

int main(void)
{
    ENV env;
    TCP listener;
    listener << Address(env["IP"], stringToInteger(env["PORT"]));
    match_making logic;
    logic.map["set"] = [&logic](vector<string> vec) -> void
    {
        logic.info[vec[0]] = vec[1];
    };
    logic(listener);
    listener.close_socket();

    return 0;
}