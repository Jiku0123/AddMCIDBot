#pragma once

#include <sqlite3.h>
#include <string>

class Database{
private:
    sqlite3* db = nullptr;

public:
    ~Database();

    bool open(const std::string& path);
    void close();

    bool initialize();

    bool register_mcid(
        const std::string& discord_id,
        const std::string& edition,
        const std::string& mcid
    );

    bool unregister_mcid(
        const std::string& discord_id,
        const std::string& edition
    );

    bool get_mcid(
        const std::string& discord_id,
        const std::string& edition,
        std::string& mcid
    );
};