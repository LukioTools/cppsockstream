
#include "socket.hpp"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


int main(int argc, char const *argv[])
{
    addrinfo hints, * res;
    memset(&hints, 0, sizeof(hints));
    char header[] = "GET / HTTP/1.1\r\nHost: www.example.com\r\nContent-Length:0\r\n\r\n";

    auto i = getaddrinfo("www.example.com", "80", &hints,  &res);
    BufferedSocket s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(!s){
        std::cerr << "Failed to create socket " << std::endl;
        return 1;
    }
    s.connect(res->ai_addr, res->ai_addrlen);
    s.send(header, sizeof(header));

    std::string imethod;
    std::string iheader;
    std::string ibody;
    size_t error = 0;

    s.read_until(imethod, "\r\n", &error); if(error) std::cout <<  "imethod: " <<  strerror(error) << std::endl;
    s.read_until(iheader, "\r\n\r\n", &error); if(error) std::cout <<  "iheader: " <<  strerror(error) << std::endl;
    s.read_until(ibody, "\r\n\r\n", &error); if(error) std::cout <<    "ibody: " <<  strerror(error) << std::endl;


    std::cout << "imethod:\n"<< imethod;
    std::cout << "iheader:\n"<< iheader;
    std::cout << "ibody:\n"<< ibody;
    std::ofstream file("get.html");

    file << ibody;
    file.close();
    freeaddrinfo(res);
    return 0;
}
