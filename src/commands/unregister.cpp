#include "commands/unregister.hpp"

#include <string>

void commands::unregister_register(dpp::cluster& bot){
    dpp::slashcommand unregister_command;

    dpp::command_option edition(
        dpp::co_string,
        "edition",
        "MC Edition",
        true
    );

    edition.add_choice(
        dpp::command_option_choice("Java", std::string("Java"))
    );

    edition.add_choice(
        dpp::command_option_choice("Bedrock", std::string("Bedrock"))
    );

    unregister_command.add_option(edition);

    bot.global_command_create(unregister_command);
}