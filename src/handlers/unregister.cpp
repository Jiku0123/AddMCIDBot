#include <string>

#include "handlers/unregister.hpp"

void handlers::unregister_handler(dpp::slashcommand_t& event, Database& db, RconClient& rcon){
    std::string discord_id = std::to_string(event.command.usr.id);
    std::string edition = std::get<std::string>(event.get_parameter("edition"));

    std::string mcid;

    if(db.get_mcid(discord_id, edition, mcid)){
        db.unregister_mcid(discord_id, edition);
    } else {

    }
}