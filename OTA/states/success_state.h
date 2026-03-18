#ifndef SUCCESS_STATE_H
#define SUCCESS_STATE_H

#include "../ota_state.h"

class SuccessState : public OTAState {
public:
    void onEnter(OTAStateMachine& ctx) override;
    void handle(OTAStateMachine& ctx) override;
    const char* name() const override { return "Success"; }
};

#endif
