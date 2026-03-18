#ifndef VERIFY_STATE_H
#define VERIFY_STATE_H

#include "../ota_state.h"

class VerifyState : public OTAState {
public:
    void onEnter(OTAStateMachine& ctx) override;
    void handle(OTAStateMachine& ctx) override;
    const char* name() const override { return "Verify_TEE"; }
};

#endif
