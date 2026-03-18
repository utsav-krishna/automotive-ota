#include "success_state.h"
#include "../ota_state_machine.h"
#include <iostream>

void SuccessState::onEnter(OTAStateMachine&) {
    std::cout << "Update successful.\n" << std::endl;
    std::cout << "---- OTA Update Process Completed ----\n";
}

void SuccessState::handle(OTAStateMachine&) {
}
