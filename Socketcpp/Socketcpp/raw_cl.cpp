#include <asm-generic/errno-base.h>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "socket.hpp"

int main(int argc, char const *argv[])
{
    /*
    Socket s = socket(AF_INET, SOCK_STREAM, 0);

    if(!s)
        throw std::runtime_error("Could not create socket");

    s.connect("127.0.0.1", 18080);
    std::string msg;
    std::getline(std::cin, msg);
    s.sendmsg(msg);


    s.close();
    */

    BufferedSocket bs = socket(AF_INET, SOCK_STREAM, 0);

    if(!bs)
        throw std::runtime_error("Could not create socket");

    bs.connect("127.0.0.1", 18080);


    std::string str; //define a string
    size_t err = 0; // define a size_t for errors


    bs.read_until(str, '\n', &err); //read untill
        if(err) throw std::runtime_error(strerror(err)); //error handle
    std::cout << "Got: " << str; //print

    str.clear(); //clear
    
    bs.read_until(str, '\n', &err); //read untill
        if(err) throw std::runtime_error(strerror(err)); //error handle
    std::cout << "Got: " << str; // print


    return 0;
}
