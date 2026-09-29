#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include <dpp/dpp.h>
#include <laserpants/dotenv-0.9.3/dotenv.h>

#include "bot.hpp"
#include "db/db.hpp"
#include "rcon/rcon_client.hpp"

int main(){
    dotenv::init();

    Database db;

    if(!db.open("../data/mc.db")){
        return 1;
    }

    if(!db.initialize()){
        return 1;
    }

    const char* TOKEN = std::getenv("DISCORD_TOKEN");
    const char* HOST = std::getenv("RCON_HOST");
    const char* PORT = std::getenv("RCON_PORT");
    const char* PASSWORD = std::getenv("RCON_PASSWORD");

    if (TOKEN == nullptr){
        std::cout << "TOKEN was not founded" << "\n";
        return 1;
    }
    if ( HOST == nullptr){
        std::cout << "HOST was not founded" << "\n";
        return 1;
    }
    if (PORT == nullptr){
        std::cout << "PORT was not founded" << "\n";
        return 1;
    }
    if (PASSWORD == nullptr){
        std::cout << "PASSWORD was not founded" << "\n";
        return 1;
    }

    int port;
    try{
        std::string port_str(PORT);
        size_t pos = 0;

        port = std::stoi(port_str, &pos);

        if(pos != port_str.size()){
            std::cout << "RCON_PORT contains invalid characters\n";
            return 1;
        }

        if(port < 1 || port > 65535){
            std::cout << "RCON_PORT must be between 1 and 65535\n";
            return 1;
        }
    }catch(const std::invalid_argument& e){
        std::cout << "RCON_PORT is not a number: " << e.what() << "\n";
        return 1;
    }catch(const std::out_of_range& e){
        std::cout << "RCON_PORT is out of range: " << e.what() << "\n";
        return 1;
    }

    RconClient rcon(HOST, port, PASSWORD);

    if(!rcon.connect()){
        std::cout << "RCON connection failed\n";
        return 1;
    }

    if(!rcon.authenticate()){
        std::cout << "RCON authentication failed\n";
        return 1;
    }

    std::cout << "RCON connected\n";

    Bot bot(TOKEN, db, rcon);
    bot.run();

    return 0;
}