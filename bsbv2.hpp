#pragma once
#include <iostream>
#include <streambuf>
#include <array>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class basic_sockbuf : public std::streambuf {
public:
    basic_sockbuf(int sockfd) : sockfd(sockfd) {
        setg(in_buffer.data(), in_buffer.data(), in_buffer.data());
        setp(out_buffer.data(), out_buffer.data() + out_buffer.size());
    }

    ~basic_sockbuf() {
        sync();
    }

protected:
    virtual int_type underflow() override {
        if (gptr() < egptr())
            return traits_type::to_int_type(*gptr());

        int bytes_received = recv(sockfd, in_buffer.data(), in_buffer.size(), 0);
        if (bytes_received <= 0)
            return traits_type::eof();

        setg(in_buffer.data(), in_buffer.data(), in_buffer.data() + bytes_received);
        return traits_type::to_int_type(*gptr());
    }

    virtual int_type overflow(int_type c = traits_type::eof()) override {
        if (!traits_type::eq_int_type(c, traits_type::eof())) {
            *pptr() = traits_type::to_char_type(c);
            pbump(1);
        }

        return flush_buffer();
    }

    virtual int sync() override {
        return flush_buffer();
    }

private:
    int flush_buffer() {
        int bytes_to_send = pptr() - pbase();
        if (bytes_to_send <= 0)
            return 0;

        int total_bytes_sent = 0;
        while (total_bytes_sent < bytes_to_send) {
            int bytes_sent = send(sockfd, out_buffer.data() + total_bytes_sent, bytes_to_send - total_bytes_sent, 0);
            if (bytes_sent <= 0)
                return traits_type::eof();

            total_bytes_sent += bytes_sent;
        }

        pbump(-bytes_to_send);
        return total_bytes_sent;
    }

private:
    int sockfd;
    std::array<char, 1024> in_buffer; // Input buffer
    std::array<char, 1024> out_buffer; // Output buffer
};

class basic_sockstream : public std::iostream {
public:
    basic_sockstream(int sockfd) : std::iostream(&buf), buf(sockfd) {}

private:
    basic_sockbuf buf;
};
