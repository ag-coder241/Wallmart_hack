#include <iostream>
#include <string>
#include <sqlite3.h>
#include <random>
#include <sstream>

using namespace std;

int main(){

    // code here

    // flow
    // each transaction will generate a unique token and we will cryptographically sign it using HMAC signature
    // all the current token are stored at user's device.
    // when the user wants to redeem the tokens will ask for device password and a QR code is generated
    // we scan the QR at the store side and we will check which are issued by us(using the HMAC signature) and which 
    // are still valid and mark the used as valid in the backend
    // this way users can benefit from the loyalty points without ever needing to reveal their identity

    
}