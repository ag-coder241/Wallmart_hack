#ifndef TOKEN_UTILS_H
#define TOKEN_UTILS_H

#include <string>

std::string generateUUID();
std::string generateHMAC(const std::string& data);

#endif
