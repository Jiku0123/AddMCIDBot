#include "register.hpp"

void handlers::register_handler(const dpp::slashcommand_t& event){
    std::string discord_id = std::to_string(event.command.usr.id);
    std::string edition = std::get<std::string>(event.get_parameter("edition"));
    std::string user_id = std::get<std::string>(event.get_parameter("mcid"));
}