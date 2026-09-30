#include "commands/unregister.hpp"

#include <string>

void commands::unregister_register(dpp::cluster& bot){
    dpp::slashcommand unregister_command;

    unregister_command.set_name("unregister");
    unregister_command.set_description("Unregister your MCID from the whitelist");
    unregister_command.set_application_id(bot.me.id);

    dpp::command_option edition(
        dpp::co_string,
        "edition",
        "MC Edition",
        true
    );

    edition.add_choice(
        dpp::command_option_choice("Java", std::string("java"))
    );

    edition.add_choice(
        dpp::command_option_choice("Bedrock", std::string("bedrock"))
    );

    unregister_command.add_option(edition);

    bot.global_command_create(unregister_command);
}