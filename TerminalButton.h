#pragma once

#include "TerminalLabel.h"


class TerminalButton : public TerminalLabel {
public:
	DECLARE_KIND(TerminalLabel, TerminalControl::Kind::BUTTON)

	template<class... Args>
	static TerminalButtonPtr Create(Args... args) {
		return std::make_shared<TerminalButton>(std::forward<Args>(args)...);
	}
	TerminalButton(const Utf8String& label, TerminalCoord position) : TerminalLabel(label, position) {}
};