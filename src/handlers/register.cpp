#include "handlers/register.hpp"
#include "mc/whitelist_handler.hpp"

#include <string>

void handlers::register_handler(const dpp::slashcommand_t& event, Database& db, RconClient& rcon){
    std::string discord_id = std::to_string(event.command.usr.id);
    std::string edition = std::get<std::string>(event.get_parameter("edition"));
    std::string mcid = std::get<std::string>(event.get_parameter("mcid"));

    WhitelistHandlers whitelist(rcon);

    std::string current_mcid;

    dpp::message already_registered;
    already_registered.set_content("The MCID for this edition has already been registered.");
    already_registered.set_flags(dpp::m_ephemeral);

    dpp::message failed_add_whitelist;
    failed_add_whitelist.set_content("Failed to add to the whitelist. Check the MCID and try again.");
    failed_add_whitelist.set_flags(dpp::m_ephemeral);

    dpp::message failed_add_db;
    failed_add_db.set_content("Failed to save your registration.");
    failed_add_db.set_flags(dpp::m_ephemeral);

    dpp::message invalid_mcid;
    invalid_mcid.set_content("Invalid MCID format.");
    invalid_mcid.set_flags(dpp::m_ephemeral);

    dpp::message success_add;
    success_add.set_content("Registered <" + mcid + "> to the whitelist.");
    success_add.set_flags(dpp::m_ephemeral);

    if(db.get_mcid(discord_id, edition, current_mcid)){
        event.reply(already_registered);
        return;
    }
    
    if(!whitelist.is_valid_mcid(edition, mcid)){
        event.reply(invalid_mcid);
        return;
    }

    if(!whitelist.add(edition, mcid)){
        event.reply(failed_add_whitelist);
        return;
    }

    if(!db.register_mcid(discord_id, edition, mcid)){
        whitelist.remove(edition, mcid);
        event.reply(failed_add_db);
        return;
    }

    event.reply(success_add);
}