#ifndef HEALTH_STATE_H
#define HEALTH_STATE_H

#include "../ota_state.h"

class HealthState : public OTAState {
public:
    void onEnter(OTAStateMachine& ctx) override;
    void handle(OTAStateMachine& ctx) override;
    const char* name() const override { return "HealthCheck"; }
};

#endif
