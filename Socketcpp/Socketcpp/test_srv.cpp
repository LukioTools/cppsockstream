#include "socket.hpp"
#include <iostream>
#include <netinet/in.h>
#include <ostream>
#include <sys/socket.h>

int main(int argc, char const *argv[])
{
    BufferedSocket s = socket(AF_INET, SOCK_STREAM, 0);

    s.bind(18080);
    s.listen(12);

    while (true) {
        sockaddr_in addr;
        socklen_t addr_len;
        s.accept(&addr, &addr_len);
        std::cout << "accepted le connection\n";


        s.close();
    }


    return 0;
}
