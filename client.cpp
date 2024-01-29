#include "Socketcpp/socket.hpp"
#include <iostream>
#include <memory>
#include <sys/socket.h>
#include "port.hpp"
int main(int argc, char const *argv[])
{
    Socket s(socket(AF_INET, SOCK_STREAM, 0));
    s.connect("127.0.0.1", PORT);
    size_t iz = 0;
    std::cout << "recv msg len..\n";
    auto red = s.recv(&iz);
    std::cout << "iz: " << iz << std::endl;
    std::cout << "read: " << red << std::endl;
    auto p = std::make_unique<char[]>(iz);
    s.recv(p.get(), iz);
    for (size_t i = 0; i < iz; i++)
        std::cout << p[i];
    std::cout << std::endl;
    
    s.sendmsg("Hello From client!");

    s.close();
    return 0;
}
