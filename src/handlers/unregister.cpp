#include "unregister.hpp"

#include <string>

void handlers::unregister_handler(dpp::slashcommand_t& event){
    std::string discord_id = std::to_string(event.command.usr.id);
    std::string edition = std::get<std::string>(event.get_parameter("edition"));
}