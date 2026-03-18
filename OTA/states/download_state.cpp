#include "download_state.h"
#include "../ota_state_machine.h"
#include "verify_state.h"
#include <iostream>

void DownloadState::onEnter(OTAStateMachine& ctx) {
    std::cout << "Downloading version "
              << ctx.newVersion << "...\n";
}

void DownloadState::handle(OTAStateMachine& ctx) {
    ctx.setState(std::make_unique<VerifyState>());
}
