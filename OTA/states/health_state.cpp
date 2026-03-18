#include "health_state.h"
#include "../ota_state_machine.h"
#include "success_state.h"
#include "rollback_state.h"
#include <iostream>

void HealthState::onEnter(OTAStateMachine&) {
    std::cout << "Health check...\n";
}

void HealthState::handle(OTAStateMachine& ctx) {

    bool healthy = true;

    if (!healthy)
        ctx.setState(std::make_unique<RollbackState>());
    else {
        ctx.storage.updateVersion(ctx.newVersion);
        ctx.activePartition = ctx.inactivePartition;
        ctx.setState(std::make_unique<SuccessState>());
    }
}
