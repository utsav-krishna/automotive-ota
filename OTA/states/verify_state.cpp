#include "verify_state.h"
#include "../ota_state_machine.h"
#include "install_state.h"
#include "error_state.h"

void VerifyState::onEnter(OTAStateMachine& ctx) {

    bool ok = ctx.tee.verifyImage(
        ctx.newSignature,
        ctx.storage.getOEMPublicKey(),
        ctx.newVersion,
        ctx.storage.getCurrentVersion());

    if (!ok)
        ctx.setState(std::make_unique<ErrorState>());
}

void VerifyState::handle(OTAStateMachine& ctx) {
    ctx.setState(std::make_unique<InstallState>());
}
