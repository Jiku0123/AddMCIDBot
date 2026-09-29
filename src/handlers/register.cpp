#include "handlers/register.hpp"

#include <string>

void handlers::register_handler(const dpp::slashcommand_t& event, Database& db, RconClient& rcon){
    std::string discord_id = std::to_string(event.command.usr.id);
    std::string edition = std::get<std::string>(event.get_parameter("edition"));
    std::string mcid = std::get<std::string>(event.get_parameter("mcid"));

    std::string current_mcid;

    dpp::message already_reg;
    already_reg.set_content("The MCID for this edition has already been registered.");
    already_reg.set_flags(dpp::m_ephemeral);

    if(db.get_mcid(discord_id, edition, current_mcid)){
        event.reply(already_reg);
        return;
    }
}