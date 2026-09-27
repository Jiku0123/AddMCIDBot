#include "bot.hpp"
#include "commands/register.hpp"
#include "handlers/register.hpp"

Bot::Bot(const std::string& token) : bot(token){
    commands::register_register(bot);
}

void Bot::event_handler(dpp::cluster& bot){
    bot.on_slashcommand([this](const dpp::slashcommand_t& event){
        std::string command_name = event.command.get_command_name();
        if(command_name == "register"){
            handlers::register_handler(event);
        }else if(command_name == "unregister"){

        }
    });
}

void Bot::run(){
    std::cout << "BOT is starting..." << "\n";
    bot.on_log(dpp::utility::cout_logger());

    event_handler(bot);

    bot.start(dpp::st_wait);
}