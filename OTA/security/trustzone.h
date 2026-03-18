#ifndef TRUSTZONE_H
#define TRUSTZONE_H

#include <string>

class TrustZoneTEE {
public:
    bool verifyImage(const std::string& signature,
                     const std::string& publicKey,
                     int newVersion,
                     int storedVersion);
};

#endif
