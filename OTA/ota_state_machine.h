#ifndef OTA_STATE_MACHINE_H
#define OTA_STATE_MACHINE_H

#include <memory>
#include <string>
#include "ota_state.h"
#include "security/secure_storage.h"
#include "security/trustzone.h"
#include "security/bootloader.h"

class OTAStateMachine {
public:
    OTAStateMachine();

    void startUpdate(int version, const std::string& signature);
    void setState(std::unique_ptr<OTAState> newState);
    void process();

    int newVersion;
    std::string newSignature;

    std::string activePartition;
    std::string inactivePartition;

    SecureStorage storage;
    TrustZoneTEE tee;
    Bootloader bootloader;

private:
    std::unique_ptr<OTAState> currentState;
};

#endif
