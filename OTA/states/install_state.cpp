#include "install_state.h"
#include "../ota_state_machine.h"
#include "reboot_state.h"
#include <iostream>

void InstallState::onEnter(OTAStateMachine& ctx) {
    std::cout << "Installing to partition "
              << ctx.inactivePartition << "\n";
}

void InstallState::handle(OTAStateMachine& ctx) {
    ctx.setState(std::make_unique<RebootState>());
}
