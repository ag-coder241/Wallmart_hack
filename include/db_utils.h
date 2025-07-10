#ifndef DB_UTILS_H
#define DB_UTILS_H

#include <string>
#include <vector>

struct Token {
    std::string uuid;
    std::string hmac;
    int points;
};

void initDatabase();
bool storeToken(const std::string& userId, const Token& token);
std::vector<Token> getUserTokens(const std::string& userId);
bool markTokenAsUsed(const std::string& uuid);

#endif
