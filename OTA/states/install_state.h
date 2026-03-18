#ifndef INSTALL_STATE_H
#define INSTALL_STATE_H

#include "../ota_state.h"

class InstallState : public OTAState {
public:
    void onEnter(OTAStateMachine& ctx) override;
    void handle(OTAStateMachine& ctx) override;
    const char* name() const override { return "Install"; }
};

#endif
