#include "../include/token_utils.h"
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <uuid/uuid.h>
#include <iostream>

std::string SecretKey = "@k@shgoy@l2004";

std::string generateUUID() {
    uuid_t uuid;
    uuid_generate(uuid);
    char uuid_str[37];
    uuid_unparse(uuid, uuid_str);
    return std::string(uuid_str);
}

std::string generateHMAC(const std::string& data) {
    using namespace CryptoPP;
    std::string mac, encoded;

    try {
        HMAC<SHA256> hmac(reinterpret_cast<const CryptoPP::byte*>(SecretKey.data()), SecretKey.size());
        StringSource(data, true, new HashFilter(hmac, new StringSink(mac)));
        StringSource(mac, true, new HexEncoder(new StringSink(encoded)));
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "HMAC Error: " << e.what() << std::endl;
    }

    return encoded;
}


