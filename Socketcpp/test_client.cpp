#include "socket.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    Socket s(AF_INET, SOCK_STREAM, 0);
    std::cout << "Socket::fd()::" << s.fd() << std::endl;
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(18080);
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
        return -1;
    }
    tryagain:
    try{
        s.connect(&serv_addr, sizeof(serv_addr));
    }
    catch(const std::runtime_error& e){
        //ffs
        std::cerr<< std::endl << e.what() << std::endl;
        return -1;
    }

    char msg[] = "Hello Server!";
    std::cout << "Sending: " << msg << std::endl;
    size_t sz = sizeof(msg);
    auto wrote = s.send(sz);
    std::cout << "Wrote: "  << wrote << '/' << sizeof(sz) << std::endl;
    s.send(msg, sz);

    s.recive(&sz);

    s.close();

    /*
    auto s = socket(AF_INET, SOCK_STREAM, 0);
    if(s <= 0)
        return -1;
    
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(18080);
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        return -2;
    
    auto e= connect(s, (const sockaddr*)&serv_addr, sizeof(serv_addr));
    if(e == -1)
        return -3;
    
    close(s);
    */
    return 0;
}
