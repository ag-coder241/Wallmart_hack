#pragma once
#include <string>

auto generate_token(double amount, const std::string &user_id) -> std::string;

// returns true on success
bool store_token(const std::string &user_id, const std::string &token);

//  returns the leftover reward points after redemption;  throws on error
//  `amount_to_redeem` is IN/OUT – it will be clamped to the actually redeemed amount
//  so the caller can see how much was redeemed.
double redeem_token(const std::string &user_id, double &amount_to_redeem);
