#ifndef SECURE_STORAGE_H
#define SECURE_STORAGE_H

#include <string>

class SecureStorage {
public:
    SecureStorage();

    std::string getOEMPublicKey() const;
    int getCurrentVersion() const;
    void updateVersion(int version);

private:
    std::string oemPublicKey;
    int currentVersion;
};

#endif
