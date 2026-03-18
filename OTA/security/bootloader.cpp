#include "bootloader.h"
#include <iostream>

bool Bootloader::secureBootVerify(const std::string& signature,
                                  const std::string& publicKey) {

    std::cout << "Bootloader: Performing secure boot...\n";

    if (signature == publicKey) {
        std::cout << "Bootloader: Secure boot passed.\n";
        return true;
    }

    std::cout << "Bootloader: Secure boot FAILED.\n";
    return false;
}
