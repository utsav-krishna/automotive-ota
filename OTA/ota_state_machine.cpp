#include "ota_state_machine.h"
#include "states/idle_state.h"
#include <iostream>

OTAStateMachine::OTAStateMachine()
    : activePartition("A"),
      inactivePartition("B"),
      newVersion(0) {}

void OTAStateMachine::startUpdate(int version,
                                  const std::string& signature) {
    newVersion = version;
    newSignature = signature;
    setState(std::make_unique<IdleState>());
}

void OTAStateMachine::setState(std::unique_ptr<OTAState> newState) {
    currentState = std::move(newState);
    std::cout << "\n--> State: " << currentState->name() << "\n";
    currentState->onEnter(*this);
}

void OTAStateMachine::process() {
    while (currentState) {
        auto* state = currentState.get();
        state->handle(*this);
        if(state == currentState.get()) {
            break;
        }
        //currentState->handle(*this);
    }
}
