#include "../include/token_utils.h"
#include "../include/db_utils.h"
#include <iostream>

using namespace std;

Token generateToken(const string& userId, int points) {
    Token token;
    token.uuid = generateUUID();
    token.points = points;
    token.hmac = generateHMAC(token.uuid + to_string(points));

    cout << "Token generated:\n";
    cout << "UUID: " << token.uuid << "\n";
    cout << "HMAC: " << token.hmac << "\n";
    cout << "Points: " << token.points << "\n";

    return token;
}
