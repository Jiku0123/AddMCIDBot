#include "db.hpp"

#include <iostream>

Database::~Database(){
    close();
}

bool Database::open(const std::string& path){
    if(sqlite3_open(path.c_str(), &db) != SQLITE_OK){
        std::cerr << "DB open error: " << sqlite3_errmsg(db) << "\n";
        close();
        return false;
    }

    return true;
}

void Database::close(){
    if(db != nullptr){
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Database::initialize(){
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS users(
            discord_id TEXT PRIMARY KEY,
            java_mcid TEXT,
            bedrock_mcid TEXT
        );
    )";

    char* error = nullptr;

    int result = sqlite3_exec(
        db,
        sql,
        nullptr,
        nullptr,
        &error
    );

    if(result != SQLITE_OK){
        std::cerr << "DB initialize error: " << error << "\n";
        sqlite3_free(error);
        return false;
    }

    return true;
}