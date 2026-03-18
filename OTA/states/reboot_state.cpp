#include "reboot_state.h"
#include "../ota_state_machine.h"
#include "health_state.h"
#include "rollback_state.h"
#include <iostream>

void RebootState::onEnter(OTAStateMachine&) {
    std::cout << "Rebooting...\n";
}

void RebootState::handle(OTAStateMachine& ctx) {

    bool bootOk = ctx.bootloader.secureBootVerify(
        ctx.newSignature,
        ctx.storage.getOEMPublicKey());

    if (!bootOk)
        ctx.setState(std::make_unique<RollbackState>());
    else
        ctx.setState(std::make_unique<HealthState>());
}
