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
#include "TerminalBorderListView.h"
#include "TerminalCheckBox.h"
#include "TerminalControlsConfig.h"


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

    auto isProfileEnable = TerminalCheckBox::Create("Time profiler ",
        TerminalCoord{ .row = backgroundWindow->Height() - 1, .col = 3 });
    isProfileEnable->SetOnChangedCallback([this](TerminalCheckBox* sender, bool isChecked) {
        TControlsConfig().profileEnable = isChecked;
        });
    isProfileEnable->SetLabelFormatSettings(FormatSettings{ .fontColor = FontColor::Yellow });
    backgroundWindow->AddControlOnBorder(isProfileEnable);

    auto isFullRender = TerminalCheckBox::Create("Full render ", TerminalCoord{ .row = backgroundWindow->Height() - 1, .col = isProfileEnable->ColEnd() + 2 });
    isFullRender->SetOnChangedCallback([this](TerminalCheckBox* sender, bool isChecked) {
        TControlsConfig().isFullRender = isChecked;
        });
    isFullRender->SetLabelFormatSettings(FormatSettings{. fontColor = FontColor::Brightblue});
    backgroundWindow->AddControlOnBorder(isFullRender);

    auto isSimpleRender = TerminalCheckBox::Create("Simple render ", TerminalCoord{ .row = backgroundWindow->Height()-1, .col = isFullRender->ColEnd() + 2});
    isSimpleRender->SetLabelFormatSettings(FormatSettings{ .fontColor = FontColor::Green });
    isSimpleRender->SetOnChangedCallback([this](TerminalCheckBox* sender, bool isChecked) {
        TControlsConfig().isSimpleRender = isChecked;
        FrameRender(true);
        });
    backgroundWindow->AddControlOnBorder(isSimpleRender);



    dbgListView = TerminalBorderListView::Create("Debug info",
        TerminalCoord{ .row = 1, .col = 89 },
        TerminalSize{ .height = 28, .width = 30 });

    dbgListView->SetBorderColor(FontColor::Magenta);
    dbgListView->SetTitleColor(FontColor::Yellow);
    backgroundWindow->AddControl(dbgListView);

    auto borderListView = TerminalBorderListView::Create("BorderListView",
        TerminalCoord{ .row = 1, .col = 69 },
        TerminalSize{ .height = 28, .width = 20 });
    borderListView->SetTitleColor(FontColor::Brightgreen);
    backgroundWindow->AddControl(borderListView);

    TControlsConfig().tp.SetCallback([this](const std::string& message) {
        dbgListView->AddItem(message);
        return true;
        });

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

    auto btnAddNewItem = TerminalButton::Create("+ Add new item", TerminalCoord{ .row = 7, .col = 5 });
    btnAddNewItem->AddClickCallback([borderListView]() {
        static int num = 0;
        borderListView->AddItem("#" + std::to_string(++num) + " message");
        return true;
        });

    auto btnRemoveLastItem = TerminalButton::Create("- Remove last item", TerminalCoord{ .row = 8, .col = 5 });

    btnRemoveLastItem->AddClickCallback([borderListView]() {
        return borderListView->RemoveLastItem();
        });
    DanilWindow->AddControl(btnAddNewItem);
    DanilWindow->AddControl(btnRemoveLastItem);
    AddWindow(DanilWindow);

    for (int i = 0; i < 175; ++i) {
        btnAddNewItem->ApplyMouseLeftClick(TerminalCoord());
    }

    auto radioButtonChanged = [rbBorderBrightcyan, rbBorderCyan, borderListView](TerminalRadioButton* sender, bool isSelected) {
        if (!isSelected) {
            return;
        }
        if (sender == rbBorderCyan.get()) {
            borderListView->SetBorderColor(FontColor::Cyan);
        }
        if (sender == rbBorderBrightcyan.get()) {
            borderListView->SetBorderColor(FontColor::Brightcyan);
        }
    };
    rbBorderCyan->SetOnChangedCallback(radioButtonChanged);
    rbBorderBrightcyan->SetOnChangedCallback(radioButtonChanged);

    auto cbBorderChanged = [borderListView](TerminalCheckBox* sender, bool isChecked) {
        borderListView->SetBorderVisible(isChecked);
    };
    cbBorder->SetOnChangedCallback(cbBorderChanged);
}

void TerminalApplication::AddWindow(TerminalWindowPtr window) {
    rootControl->AddControl(window);
}

void TerminalApplication::FrameRender(bool isFullRender) {
    if (isFullRender) {
        TControlsConfig().isFullRender = true;
    }

    canvas->Render(rootControl);

    if (isFullRender) {
        TControlsConfig().isFullRender = false;
    }
}

void TerminalApplication::OnMouseLeftClick(TerminalCoord absPosition, bool isCtrl, bool isFromDoubleClick) {
    auto clickCell = canvas->Get(absPosition);
    auto clickControl = clickCell.GetParent();
    auto clickWindow = clickControl ? clickControl->GetParentWindow() : nullptr;

    SetFocusControl(clickControl, clickWindow);

    bool isDraggingStart = TryDraggingStart(clickControl, absPosition);

    TimeProfiler& tp = TControlsConfig().tp;
    tp.Push("MoveToTop");
    bool isMoveToTop = rootControl->MoveToTop(clickWindow);
    tp.Pop("MoveToTop", isMoveToTop);
    
    bool isApplyClickOK = clickControl ? clickControl->ApplyMouseLeftClick(absPosition) : false;

    if (isMoveToTop || isApplyClickOK || isDraggingStart) {
        TimeProfiler& tp = TControlsConfig().tp;
        tp.Push("FrameRender");
        FrameRender();
        tp.Pop("FrameRender" + std::string(isFromDoubleClick ? "DC " : ""));
        tp.forceShouldNotCommit = true;
        FrameRender();
        tp.forceShouldNotCommit = false;
    }
}

void TerminalApplication::OnMouseDoubleClick(TerminalCoord absPosition, bool isCtrl) {
    OnMouseLeftClick(absPosition, isCtrl, true);
}

void TerminalApplication::OnMouseUp(TerminalCoord absPosition) {
    if (TryDraggingStop()) {
        FrameRender();
    }
}

void TerminalApplication::OnMouseMoved(TerminalCoord absPosition) {
    if (TryDragging(absPosition)) {
        FrameRender();
    }
}

void TerminalApplication::OnMouseWheeled(short value) {
    if (focusControl) {
        if (focusControl->ApplyMouseWheeled(value)) {
            FrameRender();
        }
    }
}

void TerminalApplication::OnKeyPressUpOrDown(bool isUp) {
    if (focusControl) {
        if (focusControl->ApplyKeyPressUpOrDown(isUp)) {
            FrameRender();
        }
    }
}

bool TerminalApplication::TryDraggingStart(TerminalControl* control, TerminalCoord absPosition) {
    if (control != nullptr && control->IsDraggable() && control->TryDraggingStart(absPosition)) {
        draggingControl = control;
        draggingBasePoint = absPosition;
        return true;
    }
    return false;
}

bool TerminalApplication::TryDragging(TerminalCoord absPosition) {
    if (draggingControl) {
        if (absPosition == draggingBasePoint) {
            return false;
        }
        TerminalCoord delta = absPosition - draggingBasePoint;
        if (draggingControl->TryDragging(delta)) {
            draggingBasePoint = absPosition;
        }
        return true;
    }
    return false;
}

bool TerminalApplication::TryDraggingStop() {
    if (draggingControl) {
        draggingControl->TryDraggingStop();
        draggingControl = nullptr;
        return true;
    }
    return false;
}

void TerminalApplication::SetFocusControl(TerminalControl* clickControl, TerminalWindow* clickWindow) {
    if (clickControl) {
        if (clickControl->IsFocusable()) {
            focusControl = clickControl;
        }
    }
    else if (clickWindow) {
        if (clickWindow->IsFocusable()) {
            focusControl = clickWindow;
        }
    }
}

void TerminalApplication::OnKeyEvent(const KEY_EVENT_RECORD& key) {
    std::string dir = key.bKeyDown ? "Down" : "Up";
    if (key.bKeyDown) {
        if (key.wVirtualKeyCode == VK_UP || key.wVirtualKeyCode == VK_DOWN) {
            OnKeyPressUpOrDown(key.wVirtualKeyCode == VK_UP);
        }
    }
    if (key.bKeyDown && key.uChar.AsciiChar == 'q') {
        exit(0);
    }
}

void TerminalApplication::OnMouseEvent(const MOUSE_EVENT_RECORD& mouseEvent) {
    std::string info;
    TerminalCoord absPosition{
        .row = mouseEvent.dwMousePosition.Y,
        .col = mouseEvent.dwMousePosition.X 
    };
    if (mouseEvent.dwEventFlags != 0) { // just click
        if (mouseEvent.dwEventFlags & DOUBLE_CLICK) {
            if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                OnMouseDoubleClick(absPosition, mouseEvent.dwControlKeyState& LEFT_CTRL_PRESSED);
            }
            info += "Double click";
        }
        if (mouseEvent.dwEventFlags & MOUSE_MOVED) {
            OnMouseMoved(absPosition);
            info += "Move [" + std::to_string(mouseEvent.dwMousePosition.X) + ", " + std::to_string(mouseEvent.dwMousePosition.Y) + "]";
        }
        if (mouseEvent.dwEventFlags & MOUSE_WHEELED) {
            short value = GET_WHEEL_DELTA_WPARAM(mouseEvent.dwButtonState);
            OnMouseWheeled(value);
            info += "MOUSE_WHEELED: " + std::to_string(value);
        }
        if (mouseEvent.dwEventFlags & MOUSE_HWHEELED) {
            info += "MOUSE_HWHEELED !!!: " + std::to_string(mouseEvent.dwButtonState);
        }
    }
    else {
        if (mouseEvent.dwButtonState == 0) {
            OnMouseUp(absPosition);
            info = "Up";
        }
        if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
            OnMouseLeftClick(absPosition, mouseEvent.dwControlKeyState & LEFT_CTRL_PRESSED);
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

void TerminalApplication::OnWindowResize(short rows, short cols) {
    canvas->Resize(rows, cols);
    rootControl->Resize(rows, cols);
    FrameRender(true);
}

void TerminalApplication::OnWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD& windowSizeEvent) {
    OnWindowResize(windowSizeEvent.dwSize.Y, windowSizeEvent.dwSize.X);
    //std::cout << "WINDOW_BUFFER_SIZE_RECORD: " << windowSizeEvent.dwSize.X << " : " << windowSizeEvent.dwSize.Y << std::endl;
}

void TerminalApplication::Run() {
    DWORD cc;
    INPUT_RECORD irec;

    FrameRender();
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

