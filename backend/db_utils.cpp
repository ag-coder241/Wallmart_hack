#include "../include/db_utils.h"
#include <sqlite3.h>
#include <iostream>

void initDatabase() {
    sqlite3* db;
    char* errMsg;

    int rc = sqlite3_open("rewards.db", &db);
    if (rc) {
        std::cerr << "DB init error: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::string createTable = R"sql(
        CREATE TABLE IF NOT EXISTS tokens (
            uuid TEXT PRIMARY KEY,
            user_id TEXT,
            hmac TEXT,
            points INTEGER
        );
    )sql";

    rc = sqlite3_exec(db, createTable.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Table creation error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
}

bool storeToken(const std::string& userId, const Token& token) {
    sqlite3* db;
    sqlite3_open("rewards.db", &db);

    std::string insertSQL = "INSERT INTO tokens (uuid, user_id, hmac, points) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, token.uuid.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, userId.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, token.hmac.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, token.points);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return rc == SQLITE_DONE;
}

std::vector<Token> getUserTokens(const std::string& userId) {
    std::vector<Token> tokens;
    sqlite3* db;
    sqlite3_open("rewards.db", &db);

    std::string query = "SELECT uuid, hmac, points FROM tokens WHERE user_id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Token t;
        t.uuid = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        t.hmac = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        t.points = sqlite3_column_int(stmt, 2);
        tokens.push_back(t);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return tokens;
}

bool markTokenAsUsed(const std::string& uuid) {
    sqlite3* db;
    sqlite3_open("rewards.db", &db);

    std::string delSQL = "DELETE FROM tokens WHERE uuid = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, delSQL.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, uuid.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return rc == SQLITE_DONE;
}
