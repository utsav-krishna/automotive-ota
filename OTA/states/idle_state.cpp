#include "idle_state.h"
#include "../ota_state_machine.h"
#include "download_state.h"

void IdleState::onEnter(OTAStateMachine&) {}

void IdleState::handle(OTAStateMachine& ctx) {
    ctx.setState(std::make_unique<DownloadState>());
}
