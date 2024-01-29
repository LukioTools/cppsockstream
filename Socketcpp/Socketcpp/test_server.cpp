#include "socket.hpp"
#include <asm-generic/socket.h>
#include <iostream>
#include <sys/socket.h>

int main(int argc, char const *argv[])
{
    Socket s(AF_INET, SOCK_STREAM, 0);

    int y = true;
    setsockopt(s.fd(), SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y));

    std::cout << "binding..." << std::endl;
    s.bind(18080);
    std::cout << "listening..." << std::endl;
    s.listen(12);


    while (true) {
        sockaddr_in inc_addr;
        socklen_t inc_addr_len = 0;
        std::cout << "waiting for connection..." << std::endl;
        auto client = s.accpet(&inc_addr, &inc_addr_len);
        std::cout << "Accepted a connection" << std::endl;
        size_t sz = 0;
        auto red = s.recive(&sz, sizeof(sz));

        if(red != sizeof(sz))
            std::cout << "Read: " << red << '/' <<  sizeof(sz) << std::endl;
        char* buffer = new char[sz];
        s.recive(buffer, sz);

        std::cout << "Got(" << sz << "): " << buffer << std::endl;

        char msg[] = "Hello To you!";
        sz = sizeof(msg);
        client << sz;
        client.send(msg, sz);
        client.close();
    }

    s.close();
    return 0;
}
