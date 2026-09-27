#include <cstdlib>
#include <iostream>

#include <dpp/dpp.h>
#include <laserpants/dotenv-0.9.3/dotenv.h>

#include "bot.hpp"
#include "db/db.hpp"

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

    if (TOKEN == nullptr){
        std::cout << "TOKEN was not founded" << "\n";
        return 1;
    }

    return 0;
}