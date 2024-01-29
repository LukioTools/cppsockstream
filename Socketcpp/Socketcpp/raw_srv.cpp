#include <cstddef>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include "socket.hpp"
#define PORT 18080

int nthreads = 12;

std::vector<Client> clients;

void broadcast(const std::string& msg){
    for (auto& c : clients)
        c.s.sendmsg(msg);

}

int main(int argc, char const *argv[])
{
    Socket ss = socket(AF_INET, SOCK_STREAM, 0);
    if(!ss) return -1;
    ss.bind(PORT);
    ss.listen(nthreads);
        //voi olla vaikka eri threadis 
    while (true)
    {
        auto client = ss.accept();

        std::cout << "Accepted a client" << std::endl;
        //clients.push_back(client); //add to total clients

        char om[] = "Hello\nWorld\n";
        try{
            client.s.send(om, sizeof(om));
        }
        catch(...){}
    }

    return 0;
}
