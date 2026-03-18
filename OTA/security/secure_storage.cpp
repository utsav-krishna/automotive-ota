#include "secure_storage.h"

SecureStorage::SecureStorage()
    : oemPublicKey("OEM_PUBLIC_KEY_123"),
      currentVersion(1) {}

std::string SecureStorage::getOEMPublicKey() const {
    return oemPublicKey;
}

int SecureStorage::getCurrentVersion() const {
    return currentVersion;
}

void SecureStorage::updateVersion(int version) {
    currentVersion = version;
}
