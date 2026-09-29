#include "db/db.hpp"

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

bool Database::register_mcid(
    const std::string &discord_id,
    const std::string &edition,
    const std::string &mcid
    ){
        const char* sql;

        if(edition == "java"){
            sql = R"(
                INSERT INTO users (discord_id, java_mcid)
                VALUES (?, ?)
                ON CONFLICT(discord_id)
                DO UPDATE SET java_mcid = excluded.java_mcid
                WHERE users.java_mcid IS NULL;
            )";
        }else if(edition == "bedrock"){
            sql = R"(
                INSERT INTO users (discord_id, bedrock_mcid)
                VALUES (?, ?)
                ON CONFLICT(discord_id)
                DO UPDATE SET bedrock_mcid = excluded.bedrock_mcid
                WHERE users.bedrock_mcid IS NULL;
            )";
        }else{
            return false;
        }

        sqlite3_stmt* stmt = nullptr;

        if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK){
            std::cerr << "DB prepare error: " << sqlite3_errmsg(db) << "\n";
            return false;
        }

        if(sqlite3_bind_text(stmt, 1, discord_id.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK){
            sqlite3_finalize(stmt);
            return false;
        }

        if(sqlite3_bind_text(stmt, 2, mcid.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK){
            sqlite3_finalize(stmt);
            return false;
        }

        int result = sqlite3_step(stmt);
        bool success = result == SQLITE_DONE && sqlite3_changes(db) > 0;

        sqlite3_finalize(stmt);

        return success;
}

bool Database::unregister_mcid(
    const std::string &discord_id,
    const std::string &edition
    ){
        const char* sql;

        if(edition == "java"){
            sql = "UPDATE users SET java_mcid = NULL WHERE discord_id =?;";
        }else if(edition == "bedrock"){
            sql = "UPDATE users SET bedrock_mcid = NULL WHERE discord_id =?;";
        }else{
            return false;
        }

        sqlite3_stmt* stmt = nullptr;

        if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK){
            std::cerr << "DB prepare error: " << sqlite3_errmsg(db) << "\n";
            return false;
        }

        if(sqlite3_bind_text(stmt, 1, discord_id.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK){
            sqlite3_finalize(stmt);
            return false;
        }

        int result = sqlite3_step(stmt);

        sqlite3_finalize(stmt);

        bool success = result == SQLITE_DONE && sqlite3_changes(db) > 0;

        return success;
}

bool Database::get_mcid(
    const std::string &discord_id,
    const std::string &edition,
    std::string &mcid
    ){
        const char* sql;

        if(edition == "java"){
            sql = "SELECT java_mcid FROM users WHERE discord_id = ?;";
        }else if(edition == "bedrock"){
            sql = "SELECT bedrock_mcid FROM users WHERE discord_id = ?;";
        }else{
            return false;
        }

        sqlite3_stmt* stmt = nullptr;

        if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK){
            std::cerr << "DB prepare error: " << sqlite3_errmsg(db) << "\n";
            return false;
        }

        if(sqlite3_bind_text(
            stmt, 1, discord_id.c_str(), -1, SQLITE_TRANSIENT
        ) != SQLITE_OK){
            sqlite3_finalize(stmt);
            return false;
        }

        int result = sqlite3_step(stmt);

        if(result == SQLITE_ROW){
            const unsigned char* value = sqlite3_column_text(stmt, 0);

            if(value != nullptr){
                mcid = reinterpret_cast<const char*>(value);
                sqlite3_finalize(stmt);
                return true;
            }
        }

        sqlite3_finalize(stmt);
        return false;
}