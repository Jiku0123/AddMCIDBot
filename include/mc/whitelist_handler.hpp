#pragma once

#include <string>

#include "rcon/rcon_client.hpp"

class WhitelistHandlers{
public:
    WhitelistHandlers(RconClient& mcid);

    bool add_java(const std::string& mcid);
    bool remove_java(const std::string& mcid);

    bool add_bedrock(const std::string& mcid);
    bool remove_bedrock(const std::string& mcid);

private:
    RconClient& rcon;

    bool execute_command(const std::string& command);
};