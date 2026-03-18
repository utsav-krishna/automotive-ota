#ifndef DOWNLOAD_STATE_H
#define DOWNLOAD_STATE_H

#include "../ota_state.h"

class DownloadState : public OTAState {
public:
    void onEnter(OTAStateMachine& ctx) override;
    void handle(OTAStateMachine& ctx) override;
    const char* name() const override { return "Download"; }
};

#endif
