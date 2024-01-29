#pragma once

#include <streambuf>
#include <array>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


namespace soc
{
    class basic_sockbuf : public std::streambuf {
public:
    // Constructor
    basic_sockbuf(int sockfd) : sockfd(sockfd) {
        setg(in_buffer.data(), in_buffer.data(), in_buffer.data());
        setp(out_buffer.data(), out_buffer.data() + out_buffer.size());
    }

    // Destructor
    ~basic_sockbuf() {
        sync();
    }

protected:
    // Read data from the socket
    virtual int_type underflow() override {
        if (gptr() < egptr())
            return traits_type::to_int_type(*gptr());

        int bytes_received = recv(sockfd, reinterpret_cast<char*>(in_buffer.data()), in_buffer.size(), 0);
        if (bytes_received <= 0)
            return traits_type::eof();

        setg(in_buffer.data(), in_buffer.data(), in_buffer.data() + bytes_received);
        return traits_type::to_int_type(*gptr());
    }

    // Write data to the socket
    virtual int_type overflow(int_type c) override {
        if (c != traits_type::eof()) {
            *pptr() = traits_type::to_char_type(c);
            pbump(1);
        }

        if (flush_buffer() == traits_type::eof())
            return traits_type::eof();

        return traits_type::not_eof(c);
    }

    // Synchronize the buffer with the associated character sequence
    virtual int sync() override {
        return flush_buffer();
    }

private:
    // Flush the output buffer to the socket
    int flush_buffer() {
        int bytes_to_send = pptr() - pbase();
        if (bytes_to_send <= 0)
            return 0;

        int bytes_sent = send(sockfd, reinterpret_cast<const char*>(out_buffer.data()), bytes_to_send, 0);
        if (bytes_sent <= 0)
            return traits_type::eof();

        pbump(-bytes_sent);
        return bytes_sent;
    }

private:
    int sockfd;
    std::array<char, 1024> in_buffer; // Input buffer
    std::array<char, 1024> out_buffer; // Output buffer
};
} // namespace soc
