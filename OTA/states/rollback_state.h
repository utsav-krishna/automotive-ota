#ifndef ROLLBACK_STATE_H
#define ROLLBACK_STATE_H

#include "../ota_state.h"

class RollbackState : public OTAState {
public:
    void onEnter(OTAStateMachine& ctx) override;
    void handle(OTAStateMachine& ctx) override;
    const char* name() const override { return "Rollback"; }
};

#endif
