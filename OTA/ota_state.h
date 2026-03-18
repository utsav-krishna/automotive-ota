#ifndef OTA_STATE_H
#define OTA_STATE_H

class OTAStateMachine;

class OTAState {
public:
    virtual ~OTAState() = default;
    virtual void onEnter(OTAStateMachine& ctx) = 0;
    virtual void handle(OTAStateMachine& ctx) = 0;
    virtual const char* name() const = 0;
};

#endif
