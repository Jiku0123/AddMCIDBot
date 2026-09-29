#pragma once

#include <dpp/dpp.h>

#include "db/db.hpp"
#include "rcon/rcon_client.hpp"

namespace handlers{
    void unregister_handler(const dpp::slashcommand_t& event, Database& db, RconClient& rcon);
};