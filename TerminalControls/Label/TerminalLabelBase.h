#pragma once
#include "utils/common.h"
#include "utils/utf8string.h"
#include "TerminalControl.h"

class TerminalLabelBase : public TerminalControl {
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::LABEL_BASE)

public:
    TerminalLabelBase(TerminalCoord position, TerminalSize size);
    
    virtual const Utf8String& Get() const = 0;
    virtual short Size() const = 0;

protected:
    void FlushSelf() override;
};