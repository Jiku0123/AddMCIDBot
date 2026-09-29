#include "mc/whitelist_handler.hpp"

#include <vector>

WhitelistHandlers::WhitelistHandlers(RconClient& rcon) : rcon(rcon){

}

bool WhitelistHandlers::is_valid_mcid(const std::string& edition, const std::string& mcid){
    if(edition == "java"){
        if(mcid.size() < 3 || mcid.size() > 16) return false;
        for(unsigned char c : mcid){
            if(!std::isalnum(c) && c != '_') return false;
        }
        return true;
    }

    if(edition == "bedrock"){
        if(mcid.empty() || mcid.size() > 16) return false;
        for(unsigned char c : mcid){
            if(!std::isalnum(c) && c != '_' && c != '.') return false;
        }
        return true;
    }
    return false;
}

bool WhitelistHandlers::add(const std::string& edition, const std::string& mcid){
    if(edition == "java"){
        return execute_command("whitelist add " + mcid);
    }else if(edition == "bedrock"){
        return execute_command("fwhitelist add " + mcid);
    }

    return false;
}

bool WhitelistHandlers::remove(const std::string& edition, const std::string& mcid){
    if(edition == "java"){
        return execute_command("whitelist remove " + mcid);
    }else if(edition == "bedrock"){
        return execute_command("fwhitelist remove " + mcid);
    }

    return false;
}

bool WhitelistHandlers::execute_command(const std::string& command){
    std::string response = rcon.execute(command);
    
    if(response.empty()){
        return false;
    }

    static const std::vector<std::string> success{
        "Added ", "Removed ",
        "already whitelisted", "Player is not whitelisted",
        "has been added", "has been removed",
        "was not on the whitelist"
    };

    for(const auto& s : success){
        if(response.find(s) != std::string::npos){
            return true;
        }
    }

    return false;
    
}