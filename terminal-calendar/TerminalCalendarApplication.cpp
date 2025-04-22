#define _CRT_SECURE_NO_WARNINGS

#include "common.h"
#include "TerminalApplication.h"
#include "TerminalCanvas.h"
#include "TerminalLabel.h"
#include "TerminalLabelFixedWidth.h"
#include "TerminalButton.h"
#include "TerminalWindow.h"
#include "TerminalRootControl.h"
#include "TerminalLabelSwitcher.h"
#include "DataProviders/ListDataProvider.h"
#include "TerminalListView.h"
#include "TerminalRadioButton.h"
#include "TerminalBorderListView.h"
#include "TerminalCheckBox.h"
#include "TerminalTextBox.h"
#include "TerminalGridEx.h"
#include "data-view.h"
#include "data-set.h"
#include "TerminalControlsConfig.h"
#include "Date/TerminalMonthBox.h"
#include "DataProviders/ListDataSetNumSequence.h"

#include "TerminalCalendarApplication.h"

TerminalCalendarApplication& app() {
    static TerminalCalendarApplication app;
    return app;
}

void UpdateData(TerminalCalendarApplication* self, int dh, int dw, TerminalLabelPtr sizeLabel, TerminalLabelPtr tSizeLabel) {
    RECT rect;
    GetWindowRect(self->windowHandle, &rect);
    int W = rect.right - rect.left;
    int H = rect.bottom - rect.top;
    SetWindowPos(self->windowHandle, NULL, 100, 100, W + dw, H + dh, SWP_SHOWWINDOW);

    GetWindowRect(self->windowHandle, &rect);
    W = rect.right - rect.left;
    H = rect.bottom - rect.top;

    char buf[100];
    sprintf(buf, "%dx%d", H, W);
    sizeLabel->SetText(Utf8String(buf));

    TerminalSize tsize = self->GetTerminalConsoleSize();
    sprintf(buf, "%dx%d", tsize.height, tsize.width);
    tSizeLabel->SetText(Utf8String(buf));
}
void TerminalCalendarApplication::UpdateSelectedDayCount() {
    std::string text = std::to_string(selectedDays);
    while (text.size() < 3) {
        text = " " + text;
    }
    selectedDaysCounterLabel->SetText(text);
}

void TerminalCalendarApplication::InitCalendarWindow() {
    calendarWindow = TerminalWindow::Create("", TerminalCoord{ .row = 0, .col = 0 }, TerminalSize{ .height = 38, .width = 72 });
    AddWindow(calendarWindow);

    auto yearsDataProvider = ListDataProvider::Create(yearsDataSet, yearsDataSet->GetPos(2025));
    short year_label_offset = (calendarWindow->Width() - 8) / 2;
    auto yearsLabel = TerminalLabelSwitcher::Create(yearsDataProvider, TerminalCoord{ .row = 0, .col = year_label_offset });
    yearsLabel->SetLabelFormatSettings({ .fontColor = FontColor::Green });

    calendarWindow->AddControlOnBorder(yearsLabel);

    selectedDaysCounterLabel = TerminalLabelFixedWidth::Create("  0", 3, TerminalCoord{ .row = 0, .col = 3 });
    selectedDaysCounterLabel->SetBackgroundColor(RGB::Brightcyan);
    selectedDaysCounterLabel->SetFontColor(FontColor::Black);
    calendarWindow->AddControlOnBorder(selectedDaysCounterLabel);

    // Без закругление вверху
    {
        calendarWindow->AddControlOnBorder(TerminalLabel::Create(" ", TerminalCoord{ .row = 0, .col = 0 }));
        calendarWindow->AddControlOnBorder(TerminalLabel::Create(" ", TerminalCoord{ .row = 1, .col = 0 }));

        calendarWindow->AddControlOnBorder(TerminalLabel::Create(" ", TerminalCoord{ .row = 0, .col = calendarWindow->Width() - 1 }));
        calendarWindow->AddControlOnBorder(TerminalLabel::Create(" ", TerminalCoord{ .row = 1, .col = calendarWindow->Width() - 1 }));
    }

    {
        calendarWindow->AddControl(TerminalTextBox::Create(TerminalCoord{ .row = 0, .col = 7 }, TerminalSize{ .height = 1, .width = 16 }));
        calendarWindow->AddControl(TerminalTextBox::Create(TerminalCoord{ .row = 0, .col = 27 }, TerminalSize{ .height = 1, .width = 16 }));
        auto testTextBox = TerminalTextBox::Create(TerminalCoord{ .row = 0, .col = 45 }, TerminalSize{ .height = 1, .width = 5 });
        testTextBox->SetText("01234567");
        calendarWindow->AddControl(testTextBox);
    }

    auto fillMonthsData = [this](int year) {
        size_t removeControls = calendarWindow->RemoveControlsOnBorder([](TerminalControlPtr control) {
            return control->As<TerminalMonthBox>() != nullptr;
            });
        daysSelectionLayer.Clear();
        selectedDays = 0;
        UpdateSelectedDayCount();
        assert(removeControls == 0 || removeControls == 12);

        short offset_row = 2;
        int month = 0;
        for (short row = 0; row < 4; ++row) {
            for (short col = 0; col < 3; ++col) {
                auto monthBox = TerminalMonthBox::Create(year, month++, TerminalCoord{
                    .row = offset_row + row * TerminalMonthBox::DefaultHeight(),
                    .col = col * TerminalMonthBox::DefaultWidth()
                    });
                monthBox->SetSelectionLayer(&daysSelectionLayer);
                monthBox->AddOnCellSelectedCallback([this](TerminalGridCell* sender) {
                    selectedDays += (sender->IsSelected() ? 1 : -1);
                    UpdateSelectedDayCount();
                    });
                calendarWindow->AddControlOnBorder(monthBox);
            }
        }
        };

    yearsLabel->AddChangeCallback([fillMonthsData](const Utf8String& prev, const Utf8String& current) {
        int year = (current[0].get()[0] - '0') * 1000 + (current[1].get()[0] - '0') * 100 + (current[2].get()[0] - '0') * 10 + (current[3].get()[0] - '0') * 1;
        fillMonthsData(year);
        return true;
        });

    fillMonthsData(2025);
}


void TerminalCalendarApplication::InitSearchWindow() {
    searchWindow = TerminalWindow::Create("", TerminalCoord{ .row = 2, .col =  73  }, TerminalSize{ .height = 3, .width = 72 });
    AddWindow(searchWindow);

    auto searchTextBox = TerminalTextBox::Create(TerminalCoord{ .row = 0, .col = 0 }, TerminalSize{ .height = 1, .width = 70 });
    searchWindow->AddControl(searchTextBox);
}

void TerminalCalendarApplication::InitDataWindow() {
    dataWindow = TerminalWindow::Create("", TerminalCoord{ .row = 5, .col = 73 }, TerminalSize{ .height = 33, .width = 72 });
    AddWindow(dataWindow);
    std::vector<Utf8String> header = { 
        Utf8String("             ФИО             "),
        Utf8String("Дата рождения"), 
        Utf8String("Возвраст")
    };

    auto storage = DataStorage::Create(
        FieldDesc::Int("id"),
        FieldDesc::String("name"),
        FieldDesc::Date("birthday")
    );

    storage->AddRow<int, std::string_view, storage::date>(1, "Пушкин Александр Сергеевич", storage::date(1799, 6, 6));
    storage->AddRow<int, std::string_view, storage::date>(2, "Лермонтов Михаил Юрьевич", storage::date(1814, 3, 10));
    storage->AddRow<int, std::string_view, storage::date>(3, "Толстой Лев Николаевич", storage::date(1828, 9, 9));
    storage->AddRow<int, std::string_view, storage::date>(4, "Достоевский Фёдор Михайлович", storage::date(1821, 11, 11));
    storage->AddRow<int, std::string_view, storage::date>(5, "Гоголь Николай Васильевич", storage::date(1809, 4, 1));
    storage->AddRow<int, std::string_view, storage::date>(6, "Тургенев Иван Сергеевич", storage::date(1818, 11, 9));
    storage->AddRow<int, std::string_view, storage::date>(7, "Чехов Антон Павлович", storage::date(1860, 1, 29));
    storage->AddRow<int, std::string_view, storage::date>(8, "Куприн Алексадр Иванович ", storage::date(1870, 9, 7));
    storage->AddRow<int, std::string_view, storage::date>(9, "Бунин Иван Алексеевич", storage::date(1870, 10, 22));
    storage->AddRow<int, std::string_view, storage::date>(10, "Зощенко Михаил Михайлович", storage::date(1894, 8, 9));
    storage->AddRow<int, std::string_view, storage::date>(11, "Горький Максим", storage::date(1868, 3, 28));
    storage->AddRow<int, std::string_view, storage::date>(12, "Булгаков Михал Афанасьевич", storage::date(1891, 5, 15));

    int cur_year = 2025;
    container = storage->View("name", "birthday")->AddColumn(
        FieldDesc::Int("age"), [&cur_year](const DataFieldAccessor& row) {
            storage::date b = row.GetField<storage::date>("birthday");
            return cur_year - b.year();

        }
    );
    auto dataGrid = TerminalGridEx::Create(header, container, TerminalCoord{.row = 0, .col = 0});
    dataWindow->AddControl(dataGrid);
}


TerminalCalendarApplication::TerminalCalendarApplication()
    : TerminalApplication() {

    TerminalSize size = canvas->Size();
    short rows = size.height, cols = size.width;

    InitCalendarWindow();
    InitSearchWindow();
    InitDataWindow();

    SetTerminalConsoleSize(40, 100);
}