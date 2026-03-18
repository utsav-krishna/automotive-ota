#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <string>

class Bootloader {
public:
    bool secureBootVerify(const std::string& signature,
                          const std::string& publicKey);
};

#endif
