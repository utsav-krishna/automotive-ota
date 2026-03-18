#include "ota_state_machine.h"

int main() {

    OTAStateMachine ota;

    // First scenario for successful update
    ota.startUpdate(2, "OEM_PUBLIC_KEY_123");
    ota.process();

    // Second scenario if there is a Version downgrade attempt
    ota.startUpdate(1, "OEM_PUBLIC_KEY_123");
    ota.process();

    // Third scenario is OTA image is hacked and signature validation fails
    ota.startUpdate(3, "INVALID_SIGNATURE");
    ota.process();

    return 0;
}
