#ifndef ERROR_STATE_H
#define ERROR_STATE_H

#include "../ota_state.h"

class ErrorState : public OTAState {
public:
    void onEnter(OTAStateMachine& ctx) override;
    void handle(OTAStateMachine& ctx) override;
    const char* name() const override { return "Error"; }
};

#endif
