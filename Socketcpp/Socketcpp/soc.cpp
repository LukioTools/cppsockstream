
#include <atomic>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <unistd.h>

#define PORT 18080

int nthreads = 12;
volatile std::atomic_int running_threads = 0;

void handle_shit(int server_sock_fd, int inc_sock, sockaddr_in inc_addr, socklen_t inc_addr_len){
    running_threads++;




    close(inc_sock);
    running_threads--;
}

int main(int argc, char const *argv[])
{
    
    auto server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock_fd < 0)
        return -1;

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if(bind(server_sock_fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(sockaddr_in)))
        return -2;
    

    if(listen(server_sock_fd, nthreads))
        return -3;


    while (true)
    {
        sockaddr_in inc_addr;
        socklen_t inc_addr_len = 0;
        while (running_threads >= nthreads) {
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
        auto inc_sock = accept(server_sock_fd, reinterpret_cast<sockaddr*>(&inc_addr), &inc_addr_len);
        std::thread thr(handle_shit, server_sock_fd, inc_sock, inc_addr, inc_addr_len);
        thr.detach();
    }
    


    return 0;
}
