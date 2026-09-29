#include "rcon/rcon_client.hpp"

#include <cstdint>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

namespace {
    constexpr int SERVERDATA_AUTH = 3;
    constexpr int SERVERDATA_EXECCOMMAND = 2;
};

bool send_all(int socket_fd, const void* data, size_t size){
    const char* buffer = static_cast<const char*>(data);
    size_t total = 0;

    while(total < size){
        ssize_t sent = send(
            socket_fd,
            buffer + total,
            size - total,
            0
        );

        if(sent <= 0){
            return false;
        }

        total += static_cast<size_t>(sent);
    }

    return true;
}

bool recv_all(int socket_fd, void* data, size_t size){
    char* buffer = static_cast<char*>(data);
    size_t total = 0;

    while(total < size){
        ssize_t received = recv(
            socket_fd,
            buffer + total,
            size - total,
            0
        );

        if(received <= 0){
            return false;
        }

        total += static_cast<size_t>(received);
    }

    return true;
}

void write_int32_le(char* data, int32_t value){
    uint32_t v = static_cast<uint32_t>(value);

    data[0] = static_cast<char>(v & 0xFF);
    data[1] = static_cast<char>((v >> 8) & 0xFF);
    data[2] = static_cast<char>((v >> 16) & 0xFF);
    data[3] = static_cast<char>((v >> 24) & 0xFF);
}

int32_t read_int32_le(const char* data){
    uint32_t v = 
        (static_cast<uint8_t>(data[0])) |
        (static_cast<uint8_t>(data[1]) << 8) |
        (static_cast<uint8_t>(data[2]) << 16) |
        (static_cast<uint8_t>(data[3]) << 24);

    return static_cast<int32_t>(v);
}

RconClient::RconClient(
    const std::string& host,
    int port,
    const std::string& password
    )
        : host(host), port(port), password(password)
           
    {
}

bool RconClient::connect(){
    struct addrinfo hints{};
    struct addrinfo* result = nullptr;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    std::string port_str = std::to_string(port);

    int status = getaddrinfo(
        host.c_str(),
        port_str.c_str(),
        &hints,
        &result
    );

    if(status != 0){
        return false;
    }

    int fd = socket(
        result->ai_family,
        result->ai_socktype,
        result->ai_protocol
    );

    if(fd == -1){
        freeaddrinfo(result);
        return false;
    }

    if(::connect(
        fd,
        result->ai_addr,
        result->ai_addrlen
    ) == -1){
        close(fd);
        freeaddrinfo(result);
        return false;
    }

    socket_fd = fd;

    freeaddrinfo(result);

    return true;
}

bool RconClient::authenticate(){
    int request_id = 1;

    if(!send_packet(request_id, SERVERDATA_AUTH, password)){
        return false;
    }

    char header[4];
    if(!recv_all(socket_fd, header, 4)){
        return false;
    }
    int32_t length = read_int32_le(header);

    std::string body_buf(length, '\0');
    if(!recv_all(socket_fd, &body_buf[0], length)){
        return false;
    }

    int32_t received_id = read_int32_le(&body_buf[0]);

    return received_id != -1;

}

bool RconClient::send_packet(int request_id, int type, const std::string& body){
    int32_t length = 4 + 4 + body.size() + 2;

    std::string packet(4 + length, '\0');

    write_int32_le(&packet[0], length);
    write_int32_le(&packet[4], request_id);
    write_int32_le(&packet[8], type);

    std::memcpy(
        &packet[12],
        body.data(),
        body.size()
    );

    return send_all(
        socket_fd,
        packet.data(),
        packet.size()
    );
}

std::string RconClient::execute(const std::string& command){
    int request_id = 2;
    if(!send_packet(request_id, SERVERDATA_EXECCOMMAND, command)){
        return "";
    }

    char header[4];
    if(!recv_all(socket_fd, header, 4)){
        return "";
    }
    int32_t length = read_int32_le(header);

    std::string body_buf(length, '\0');
    if(!recv_all(socket_fd, &body_buf[0], length)){
        return "";
    }

    return body_buf.substr(8, length - 8 - 2);
}

void RconClient::disconnect(){
    if(socket_fd != -1){
        close(socket_fd);
        socket_fd = -1;
    }
}