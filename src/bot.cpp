#include "bot.hpp"
#include "commands/register.hpp"
#include "commands/unregister.hpp"
#include "handlers/register.hpp"
#include "handlers/unregister.hpp"

Bot::Bot(const std::string& token, Database& db, RconClient& rcon) : bot(token), db(db), rcon(rcon){
}

void Bot::event_handler(dpp::cluster& bot){
    bot.on_slashcommand([this](const dpp::slashcommand_t& event){
        std::string command_name = event.command.get_command_name();
        if(command_name == "register"){
            handlers::register_handler(event, db, rcon);
        }else if(command_name == "unregister"){
            handlers::unregister_handler(event, db, rcon);
        }
    });
}

void Bot::run(){
    std::cout << "BOT is starting..." << "\n";
    bot.on_log(dpp::utility::cout_logger());

    event_handler(bot);

    bot.on_ready([this](const dpp::ready_t& event){
        if(dpp::run_once<struct register_bot_commands>()){
            commands::register_register(bot);
            commands::unregister_register(bot);
        }
    });

    bot.start(dpp::st_wait);
}