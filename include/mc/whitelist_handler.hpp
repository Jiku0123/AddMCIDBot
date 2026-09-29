#pragma once

#include <string>

#include "rcon/rcon_client.hpp"

class WhitelistHandlers{
public:
    WhitelistHandlers(RconClient& rcon);

    bool is_valid_mcid(const std::string& edition, const std::string& mcid);

    bool add(const std::string& edition, const std::string& mcid);

    bool remove(const std::string& edition, const std::string& mcid);

private:
    RconClient& rcon;

    bool execute_command(const std::string& command);
};