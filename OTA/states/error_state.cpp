#include "error_state.h"
#include "../ota_state_machine.h"
#include <iostream>

void ErrorState::onEnter(OTAStateMachine& ctx) {
    std::cout << "ERROR: Update failed. Staying on partition "
              << ctx.activePartition << "\n";
}

void ErrorState::handle(OTAStateMachine&) {

}
