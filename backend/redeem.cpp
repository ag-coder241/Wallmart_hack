#include "../include/db_utils.h"
#include "../include/token_utils.h"
#include <iostream>
#include <vector>

using namespace std;

void redeemPoints(const string& userId, int redeemAmount) {
    auto tokens = getUserTokens(userId);
    int available = 0;
    for (auto& t : tokens) {
        // Verify HMAC for each token
        string expected = generateHMAC(t.uuid + to_string(t.points));
        if (expected != t.hmac) {
            cout << "Tampered token detected. Skipping UUID: " << t.uuid << "\n";
            continue;
        }
        available += t.points;
    }

    if (available < redeemAmount) {
        cout << " Not enough total points. You have " << available << ", need " << redeemAmount << ".\n";
        return;
    }

    cout << "Redeeming " << redeemAmount << " points from your wallet.\n";

    // Go through tokens and subtract points
    int needed = redeemAmount;
    for (auto& t : tokens) {
        if (needed <= 0) break;

        int use = min(needed, t.points);
        needed -= use;

        // Mark token as used
        markTokenAsUsed(t.uuid);

        if (t.points > use) {
            // Generate new token for remaining
            int remaining = t.points - use;
            Token newToken;
            newToken.uuid = generateUUID();
            newToken.points = remaining;
            newToken.hmac = generateHMAC(newToken.uuid + to_string(remaining));
            storeToken(userId, newToken);

            cout << "Token " << t.uuid << " partially used. Remaining " << remaining << " issued in new token.\n";
        }
    }

    cout << "Redemption complete.\n";
}


