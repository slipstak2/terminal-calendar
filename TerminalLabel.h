#pragma once
#include "utils/common.h"
#include "TerminalControl.h"

class TerminalLabel : public TerminalControl {
public:
	template<class... Args>
	static TerminalLabelPtr Create(Args... args) {
		return std::make_shared<TerminalLabel>(std::forward<Args>(args)...);
	}
	TerminalLabel(const Utf8String& label, TerminalCoord position);

protected:
	void FlushSelf() override;
protected:
	Utf8String text;
	TerminalCoord position; // remove
};