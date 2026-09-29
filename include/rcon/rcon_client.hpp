#pragma once

#include <string>

class RconClient{
private:
    int socket_fd = -1;
    std::string host;
    int port;
    std::string password;
    bool send_packet(int request_id, int type, const std::string& body);
public:
    RconClient(
        const std::string& host,
        int port,
        const std::string& password
    );

    bool connect();
    bool authenticate();
    std::string execute(const std::string& command);
    void disconnect();
};