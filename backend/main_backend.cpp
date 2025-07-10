#include "../include/db_utils.h"
#include "../include/token_utils.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// Forward declarations
Token generateToken(const string& userId, int points);
void storeUserToken(const string& userId, const Token& token);
void redeemPoints(const string& userId, int redeemAmount);

int main() {
    initDatabase();

    while (true) {
        cout << "\n Choose an option:\n";
        cout << "1. Enter Purchase Amount\n";
        cout << "2. Redeem Points\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string userId;
            double purchaseAmount;
            cout << "👤 Enter User ID: ";
            cin >> userId;
            cout << "💳 Enter Purchase Amount (₹): ";
            cin >> purchaseAmount;

            int rewardPoints = static_cast<int>(round(purchaseAmount * 0.01));
            Token token = generateToken(userId, rewardPoints);
            storeUserToken(userId, token);

        } else if (choice == 2) {
            string userId;
            int redeem;
            cout << "👤 Enter User ID: ";
            cin >> userId;
            cout << "💰 Enter points to redeem: ";
            cin >> redeem;
            redeemPoints(userId, redeem);

        } else if (choice == 3) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "❗Invalid choice.\n";
        }
    }
    return 0;
}











