#include <string>

#include "handlers/unregister.hpp"
#include "mc/whitelist_handler.hpp"

void handlers::unregister_handler(const dpp::slashcommand_t& event, Database& db, RconClient& rcon){
    std::string discord_id = std::to_string(event.command.usr.id);
    std::string edition = std::get<std::string>(event.get_parameter("edition"));
    std::string mcid;

    WhitelistHandlers whitelist(rcon);

    dpp::message already_unregistered;
    already_unregistered.set_content("The MCID for this edition has already been unregistered.");
    already_unregistered.set_flags(dpp::m_ephemeral);

    dpp::message failed_remove_whitelist;
    failed_remove_whitelist.set_content("Failed to remove from the whitelist. Check the MCID and try again.");
    failed_remove_whitelist.set_flags(dpp::m_ephemeral);

    dpp::message failed_remove_db;
    failed_remove_db.set_content("Failed to save your unregistration");
    failed_remove_db.set_flags(dpp::m_ephemeral);

    if(!db.get_mcid(discord_id, edition, mcid)){
        event.reply(already_unregistered);
        return;
    }

    dpp::message success_remove;
    success_remove.set_content("Unregistered <" + mcid + "> from the whitelist.");
    success_remove.set_flags(dpp::m_ephemeral);

    if(!whitelist.remove(edition, mcid)){
        event.reply(failed_remove_whitelist);
        return;
    }

    if(!db.unregister_mcid(discord_id, edition)){
        whitelist.add(edition, mcid);
        event.reply(failed_remove_db);
        return;
    }

    event.reply(success_remove);
}