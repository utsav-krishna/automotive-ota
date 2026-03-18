#include "rollback_state.h"
#include "../ota_state_machine.h"
#include <iostream>

void RollbackState::onEnter(OTAStateMachine& ctx) {
    std::cout << "Rollback to partition "
              << ctx.activePartition << "\n";
}

void RollbackState::handle(OTAStateMachine&) {
}
