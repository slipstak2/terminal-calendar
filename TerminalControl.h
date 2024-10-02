#pragma once

#include "utils/common.h"
#include "TerminalRectangle.h"
#include "TerminalCell.h"

class TerminalControl : public TerminalRectangle {
public:
	TerminalControl(TerminalCoord lu, TerminalSize size);
	void AddControl(TerminalControlPtr control);

	template<class ...Args>
	TerminalCell CreateCell(Args... args) {
		TerminalCell cell(std::forward<Args>(args)...);
		cell.SetParent(this);
		return cell;
	}
	const TerminalCell& Get(short row, short col);

protected:
	void Flush();
	void FlushControls();
	virtual void FlushSelf() = 0;

protected:
	TerminalControlPtr parent = nullptr;
	std::vector<TerminalControlPtr> controls;
	std::vector<std::vector<TerminalCell>> data;

};