#pragma once

#include <dpp/dpp.h>
#include <string>

#include "rcon/rcon_client.hpp"
#include "db/db.hpp"

class Bot{
public:
    explicit Bot(const std::string& token, Database& db, RconClient& rcon);
    void event_handler(dpp::cluster& bot);
    void run();
private:
    dpp::cluster bot;
    Database& db;
    RconClient& rcon;
};