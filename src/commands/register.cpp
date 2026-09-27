#include "register.hpp"

#include <string>

void commands::register_register(dpp::cluster& bot){
    dpp::slashcommand register_command;

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
        dpp::command_option_choice("Bedrock", std::string("Bedrock"))
    );

    dpp::command_option mcid(
        dpp::co_string,
        "mcid",
        "Type MCID",
        true
    );

    register_command.add_option(edition);
    register_command.add_option(mcid);

    bot.global_command_create(register_command);
}