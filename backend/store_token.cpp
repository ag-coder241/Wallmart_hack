#include "../include/db_utils.h"
#include <iostream>

using namespace std;

void storeUserToken(const string& userId, const Token& token) {
    if (storeToken(userId, token)) {
        cout << "Token stored in DB for user: " << userId << "\n";
    } else {
        cout << "Failed to store token.\n";
    }
}

