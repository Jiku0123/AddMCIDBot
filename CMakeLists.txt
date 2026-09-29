cmake_minimum_required(VERSION 3.20)

set(project_name add_mcid_bot)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(${project_name})

find_package(DPP REQUIRED)
find_package(laserpants_dotenv REQUIRED)

add_executable(${project_name}
    src/main.cpp
    src/bot.cpp
    src/db/db.cpp
    src/rcon/rcon_client.cpp
    src/mc/whitelist_handler.cpp
    src/commands/register.cpp
    src/commands/unregister.cpp
    src/handlers/register.cpp
    src/handlers/unregister.cpp
)

target_link_libraries(${project_name} PRIVATE
    dpp::dpp
    laserpants::dotenv
    sqlite3
)

target_include_directories(${project_name} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)