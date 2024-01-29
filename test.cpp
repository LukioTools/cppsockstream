#include <asm-generic/socket.h>
#include <cstdio>
#include <memory>
#include <sys/socket.h>
#include "Socketcpp/socket.hpp"
#include <iostream>
#include "port.hpp"
#include "bsbv2.hpp"


int main(int argc, char const *argv[])
{
    auto s = Socket(socket(AF_INET, SOCK_STREAM, 0));

    s.setopt(SO_REUSEADDR);

    while (true)
        try
        {
        s.bind(PORT);
        break;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

    
    s.listen(3);

    while (true) {
        auto c = s.accept();
        
        basic_sockbuf reqbuf(c.s);
        std::iostream req(&reqbuf);

        char msg[] = "Hello from server!";
        size_t l =sizeof(msg); 
        std::cout << "Sent msg\n";


        //req.write((char*)&l, sizeof(l));
        req << l;
        req.write(msg, l);
        req.sync();



        std::cout << "reciving msg\n";
        req.read((char*)&l, sizeof(l));
        std::cout << "msg len: " << l << "\n";
        auto p = std::make_unique<char[]>(l);
        req.read(p.get(), l);
        for (size_t i = 0; i < l; i++)
            std::cout << p[i];
        std::cout << std::endl;

    }
    
    

    
    //req << l;
    //req << msg;

    return 0;
}
