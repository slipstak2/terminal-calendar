#include "utils/common.h"
#include "TerminalApplication.h"
#include "TerminalCanvas.h"
#include "TerminalLabel.h"
#include "TerminalButton.h"
#include "TerminalWindow.h"
#include "TerminalRootControl.h"
#include "TerminalLabelSwitcher.h"
#include "ListDataProvider.h"
#include "TerminalListView.h"
#include "TerminalRadioButton.h"
#include "TerminalGroupBox.h"
#include "TerminalCheckBox.h"

#include "TimeProfiler.h"

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

TerminalApplication::TerminalApplication()  {
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
    rootControl = TerminalRootControl::Create(rows, cols);
    canvas = TerminalCanvas::Create(outputHandle, rows, cols);

    auto backgroundWindow = TerminalWindow::Create("", TerminalCoord{ .row = 0, .col = 0 }, TerminalSize{ .height = rows, .width = cols });
    AddWindow(backgroundWindow);

    auto dbgGroupBox = TerminalGroupBox::Create(
        "Debug info", 
        TerminalCoord{ .row = 1, .col = 89 }, 
        TerminalSize{ .height = 28, .width = 30 });
    backgroundWindow->AddControl(dbgGroupBox);

    dbgGroupBox->SetBorderColor(FontColor::Magenta);
    dbgGroupBox->SetTitleColor(FontColor::Yellow);

    debugListView = TerminalListView::Create(
        TerminalCoord{ .row = 0, .col = 0 }, 
        TerminalSize{ .height = dbgGroupBox->Height() - 2, .width = dbgGroupBox->Width() - 2 });
    dbgGroupBox->AddControl(debugListView);

    auto IvanWindow = TerminalWindow::Create("Ivan", TerminalCoord{ .row = 3, .col = 4 }, TerminalSize{ .height = 15, .width = 40 });
    IvanWindow->SetBorderColor(FontColor::Yellow);

    std::vector<Utf8String> months{ "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль" , "Август" , "Сентябрь" , "Октябрь" , "Ноябрь" , "Декабрь"};
    auto MonthDataProviderPtr = ListDataProvider::Create(months);
    MonthDataProviderPtr->Next();
    MonthDataProviderPtr->Next();

    auto MonthLabel = TerminalLabelSwitcher::Create(MonthDataProviderPtr, TerminalCoord{ .row = 3, .col = 3 });
    IvanWindow->AddControl(MonthLabel);

    AddWindow(IvanWindow);

    auto IgorWindow = TerminalWindow::Create("Igor", TerminalCoord{ .row = 10, .col = 15 }, TerminalSize{ .height = 13, .width = 45 });
    IgorWindow->SetBorderColor(FontColor::Green);

    std::vector<Utf8String> family{ "Маша", "Верунчик или Берунчик", "Юрик$on", "Митрофан" };
    auto familyDataProvider= ListDataProvider::Create(family);
    auto LRLabel = TerminalLabelSwitcher::Create(familyDataProvider, TerminalCoord{ .row = 3, .col = 3 });
    LRLabel->SetLabelFormatSettings({ .textStyle = TextStyle::Overline });
    IgorWindow->AddControl(LRLabel);

    AddWindow(IgorWindow);

    auto DanilWindow = TerminalWindow::Create("Danil", TerminalCoord{ .row = 5, .col = 12 }, TerminalSize{ .height = 22, .width = 45 });
    DanilWindow->SetBorderColor(FontColor::Red);

    auto rbBorderCyan = TerminalRadioButton::Create("Border Cyan", TerminalCoord{ .row = 2, .col = 5 });
    DanilWindow->AddControl(rbBorderCyan);

    auto rbBorderBrightcyan = TerminalRadioButton::Create("Border Bright cyan", TerminalCoord{ .row = 3, .col = 5 });
    DanilWindow->AddControl(rbBorderBrightcyan);

    auto cbBorder = TerminalCheckBox::Create("Border visible", TerminalCoord{ .row = 5, .col = 5 });
    cbBorder->SetChecked(true);
    DanilWindow->AddControl(cbBorder);
    AddWindow(DanilWindow);

    auto radioButtonChanged = [rbBorderBrightcyan, rbBorderCyan, dbgGroupBox](TerminalRadioButton* sender, bool isSelected) {
        if (!isSelected) {
            return;
        }
        if (sender == rbBorderCyan.get()) {
            dbgGroupBox->SetBorderColor(FontColor::Cyan);
        }
        if (sender == rbBorderBrightcyan.get()) {
            dbgGroupBox->SetBorderColor(FontColor::Brightcyan);
        }
    };
    rbBorderCyan->SetOnChangedCallback(radioButtonChanged);
    rbBorderBrightcyan->SetOnChangedCallback(radioButtonChanged);

    auto cbBorderChanged = [dbgGroupBox](TerminalCheckBox* sender, bool isChecked) {
        dbgGroupBox->SetBorderVisible(isChecked);
    };
    cbBorder->SetOnChangedCallback(cbBorderChanged);
}

void TerminalApplication::AddWindow(TerminalWindowPtr window) {
    rootControl->AddControl(window);
}

void TerminalApplication::FullRender() {
    canvas->Render(rootControl);
}

void TerminalApplication::OnMouseLeftClick(short row, short col, bool isCtrl) {
    auto clickCell = canvas->Get(row, col);
    auto clickControl = clickCell.GetParent();
    auto clickWnd = clickControl ? clickControl->GetParentWindow() : nullptr;

    TimeProfiler tp;
    bool isMoveToTop = rootControl->MoveToTop(clickWnd);
    if (isMoveToTop) {
        debugListView->AddItem("MoveToTop: " + tp.GetStr());
    }
    
    bool isApplyClickOK = clickControl ? clickControl->ApplyMouseLeftClick() : false;

    if (isMoveToTop || isApplyClickOK) {
        tp.Get();
        FullRender();
        debugListView->AddItem("FullRender: " + tp.GetStr());
        FullRender();
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

