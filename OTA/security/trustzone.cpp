#include "trustzone.h"
#include <iostream>

bool TrustZoneTEE::verifyImage(const std::string& signature,
                               const std::string& publicKey,
                               int newVersion,
                               int storedVersion) {

    std::cout << "TrustZone: Verifying signature...\n";

    if (signature != publicKey) {
        std::cout << "TrustZone: Signature mismatch.\n" << std::endl;
        std::cout << "----- OTA Update Failed: Signature verification failed -----\n";
        return false;
    }

    if (newVersion <= storedVersion) {
        std::cout << "TrustZone: Anti-rollback triggered.\n" << std::endl;
        std::cout << "----- OTA Update Failed: Version " << newVersion
                  << " is not newer than current version "
                  << storedVersion << " -----\n";
        return false;
    }

    std::cout << "TrustZone: Verification successful.\n";
    return true;
}
