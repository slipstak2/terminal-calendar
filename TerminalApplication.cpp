#include "utils/common.h"
#include "TerminalApplication.h"
#include "TerminalCanvas.h"
#include "TerminalLabel.h"
#include "TerminalButton.h"
#include "TerminalWindow.h"
#include "ListDataProvider.h"

void MyErrorExit(const char* s) {
	printf("Fatal: %s\n", s);
	exit(1);
}

void GetWindowSize(HANDLE outputHandle, short& rows, short& cols) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(outputHandle, &csbi);
	cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void ShowConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

TerminalApplication::TerminalApplication() {
	inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	if (inputHandle == NULL) {
		MyErrorExit("GetStdHandle");
	}
	outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (outputHandle == NULL) {
		MyErrorExit("GetStdHandle");
	}

	DWORD fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(inputHandle, fdwMode)) {
		MyErrorExit("SetConsoleMode");
	}
	ShowConsoleCursor(false);

	short rows, cols;
	GetWindowSize(outputHandle, rows, cols);
	canvas = TerminalCanvas::Create(outputHandle, rows, cols);

	AddWindow(CreateBackgroundWindow(rows, cols));

	auto IvanWindow = CreateTerminalWindow(3, 4, 15, 40);
	IvanWindow->SetName("Ivan");
	IvanWindow->SetBackground(TerminalCell(' ', FontColor::Brightyellow));
	IvanWindow->SetBorderColor(FontColor::Yellow);

	auto IvanLabel = TerminalLabel::Create("Иван", TerminalCoord{ .row = 0, .col = 5 });
	IvanWindow->AddControl(IvanLabel);


	auto leftClickCallback = []() {
		//std::cout << "left click" << std::endl;
		return true;
		};
	auto rightClickCallback = []() {
		//std::cout << "right click" << std::endl;
		return true;
		};

	// https://www.utf8icons.com/character/9656/black-right-pointing-small-triangle
    // https://www.utf8icons.com/character/9658/black-right-pointing-pointer
	auto MonthBtnLeft = TerminalButton::Create("◀ ", TerminalCoord{ .row = 3, .col = 3 });
	MonthBtnLeft->AddClickCallback(leftClickCallback);
	IvanWindow->AddControl(MonthBtnLeft);

	std::vector<Utf8String> months{ "Январь", "Февраль", "Март" };
	auto MonthDataProviderPtr = ListDataProvider::Create(months);
	//MonthDataProviderPtr->Next();
	//MonthDataProviderPtr->Next();
	auto MonthLabel = TerminalLabel::Create(MonthDataProviderPtr, TerminalCoord{ .row = 3, .col = 5 });
	IvanWindow->AddControl(MonthLabel);

	auto MonthBtnRight = TerminalButton::Create(" ▶", TerminalCoord{ .row = 3, .col = 5 + MonthLabel->Size()});
	MonthBtnRight->AddClickCallback(rightClickCallback);
	IvanWindow->AddControl(MonthBtnRight);

	AddWindow(IvanWindow);

	auto IgorWindow = CreateTerminalWindow(10, 15, 13, 45);
	IgorWindow->SetName("Igor");
	IgorWindow->SetBackground(TerminalCell(' ', FontColor::Brightcyan));
	IgorWindow->SetBorderColor(FontColor::Green);
	auto IgorLabel = TerminalLabel::Create("Игорь", TerminalCoord{ .row = 0, .col = 3 });
	IgorWindow->AddControl(IgorLabel);
	AddWindow(IgorWindow);

	auto DanilWindow = CreateTerminalWindow(5, 12, 22, 45);
	DanilWindow->SetName("Danil");
	DanilWindow->SetBackground(TerminalCell(' ', FontColor::Brightred));
	DanilWindow->SetBorderColor(FontColor::Red);

	auto DanilLabel = TerminalLabel::Create("Danil", TerminalCoord{ .col = 3 });
	DanilWindow->AddControl(DanilLabel);
	AddWindow(DanilWindow);
}

void TerminalApplication::AddWindow(TerminalWindowPtr window) {
	windows.push_back(window);
}

void TerminalApplication::FullRender() {
	for (const auto& wnd : windows) {
		canvas->AddWindow(wnd);
	}
	canvas->Render();
}

void TerminalApplication::OnMouseLeftClick(short row, short col, bool isCtrl) {
	auto clickCell = canvas->Get(row, col);
	auto clickParent = clickCell.GetParent();

	clickParent->ApplyMouseLeftClick();

	auto clickWnd = clickParent->As<TerminalWindow>();
	for (size_t idx = 0; idx < windows.size(); ++idx) {
		if (windows[idx].get() == clickWnd) {
			if (idx == 0) {
				break;
			}
			while (idx + 1 < windows.size()) {
				swap(windows[idx], windows[idx + 1]);
				idx++;
			}
			FullRender();
			break;
		}
	}
}

void TerminalApplication::OnKeyEvent(const KEY_EVENT_RECORD& key) {
	std::string dir = key.bKeyDown ? "Down" : "Up";
	if (key.bKeyDown && key.uChar.AsciiChar == 'q') {
		exit(0);
	}
}

void TerminalApplication::OnMouseEvent(const MOUSE_EVENT_RECORD& mouseEvent) {
	std::string info;
	if (mouseEvent.dwEventFlags != 0) { // just click
		if (mouseEvent.dwEventFlags & DOUBLE_CLICK) {
			info += "Double click";
		}
		if (mouseEvent.dwEventFlags & MOUSE_MOVED) {
			info += "Move [" + std::to_string(mouseEvent.dwMousePosition.X) + ", " + std::to_string(mouseEvent.dwMousePosition.Y) + "]";
		}
		if (mouseEvent.dwEventFlags & MOUSE_WHEELED) {
			long value0 = GET_WHEEL_DELTA_WPARAM(mouseEvent.dwButtonState);
			long value = HIWORD(mouseEvent.dwButtonState);
			info += "MOUSE_WHEELED: " + std::to_string(value);
		}
		if (mouseEvent.dwEventFlags & MOUSE_HWHEELED) {
			info += "MOUSE_HWHEELED !!!: " + std::to_string(mouseEvent.dwButtonState);
		}
	}
	else {
		if (mouseEvent.dwButtonState == 0) {
			info = "Up";
		}
		if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
			OnMouseLeftClick(mouseEvent.dwMousePosition.Y, mouseEvent.dwMousePosition.X, mouseEvent.dwControlKeyState & LEFT_CTRL_PRESSED);
			info = "LeftButton";
		}
		if (mouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
			info = "RightButton";
		}
		if (mouseEvent.dwButtonState == FROM_LEFT_2ND_BUTTON_PRESSED) {
			info = "FROM_LEFT_2ND_BUTTON_PRESSED";
		}
		if (mouseEvent.dwButtonState == FROM_LEFT_3RD_BUTTON_PRESSED) {
			info = "FROM_LEFT_3RD_BUTTON_PRESSED";
		}
		if (mouseEvent.dwButtonState == FROM_LEFT_4TH_BUTTON_PRESSED) {
			info = "FROM_LEFT_4TH_BUTTON_PRESSED";
		}
	}
	//std::cout << "OnMouseEvent: " << info << std::endl;
}

void TerminalApplication::OnWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD& windowSizeEvent) {
	//std::cout << "WINDOW_BUFFER_SIZE_RECORD: " << windowSizeEvent.dwSize.X << " : " << windowSizeEvent.dwSize.Y << std::endl;
}

void TerminalApplication::Run() {
	DWORD cc;
	INPUT_RECORD irec;

	FullRender();
	for (;;) {
		ReadConsoleInput(inputHandle, &irec, 1, &cc);
		if (irec.EventType == KEY_EVENT) {
			KEY_EVENT_RECORD& keyEvent = (KEY_EVENT_RECORD&)irec.Event;
			OnKeyEvent(keyEvent);
		}
		if (irec.EventType == MOUSE_EVENT) {
			MOUSE_EVENT_RECORD& mouseEvent = (MOUSE_EVENT_RECORD&)irec.Event;
			OnMouseEvent(mouseEvent);
		}
		if (irec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
			WINDOW_BUFFER_SIZE_RECORD& windowSizeEvent = (WINDOW_BUFFER_SIZE_RECORD&)irec.Event;
			OnWindowResizeEvent(windowSizeEvent);
		}
	}
}

