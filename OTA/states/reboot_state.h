#ifndef REBOOT_STATE_H
#define REBOOT_STATE_H

#include "../ota_state.h"

class RebootState : public OTAState {
public:
    void onEnter(OTAStateMachine& ctx) override;
    void handle(OTAStateMachine& ctx) override;
    const char* name() const override { return "Reboot"; }
};

#endif
