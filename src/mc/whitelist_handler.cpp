#include "mc/whitelist_handler.hpp"

WhitelistHandlers::WhitelistHandlers(RconClient& rcon) : rcon(rcon){

}

bool WhitelistHandlers::add_java(const std::string& mcid){
    return execute_command("whitelist add " + mcid);
}

bool WhitelistHandlers::remove_java(const std::string& mcid){
    return execute_command("whitelist remove " + mcid);
}

bool WhitelistHandlers::add_bedrock(const std::string& mcid){
    return execute_command("fwhitelist add " + mcid);
}

bool WhitelistHandlers::remove_bedrock(const std::string& mcid){
    return execute_command("fwhitelist remove " + mcid);
}

bool WhitelistHandlers::execute_command(const std::string& command){
    std::string response = rcon.execute(command);
}