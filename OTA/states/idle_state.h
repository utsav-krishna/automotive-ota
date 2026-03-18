#ifndef IDLE_STATE_H
#define IDLE_STATE_H

#include "../ota_state.h"

class IdleState : public OTAState {
public:
    void onEnter(OTAStateMachine& ctx) override;
    void handle(OTAStateMachine& ctx) override;
    const char* name() const override { return "Idle"; }
};

#endif
