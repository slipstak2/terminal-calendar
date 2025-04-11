#include "TerminalCell.h"
#include "TerminalControl.h"
#include "TerminalControlsConfig.h"

void TerminalCell::SetParent(TerminalControl* newParent) {
    parent = newParent;
}

const FormatSettings& TerminalCell::GetFormatSettings() const {
    if (formatSettings) {
        return *formatSettings;
    }
    if (parent) {
        return parent->GetFormatSettings();
    }
    return FormatSettings::Default;
}

void TerminalCell::SetFormatSettings(const FormatSettings* newFormatSettings) {
    formatSettings = newFormatSettings;
}

const TerminalControl* TerminalCell::GetParent() const {
    return parent;
}

TerminalControl* TerminalCell::GetParent() {
    return parent;
}

// https://habr.com/ru/articles/119436/
// 
// 
//   SetConsoleTextAttribute(hConsole,
// FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_BLUE);
// 
// https://habr.com/ru/articles/119436/
// https://habr.com/ru/companies/macloud/articles/558316/
void TerminalCell::Render() const {
    auto fmtSettings = GetFormatSettings();

    if (TControlsConfig().isSimpleRender) {
        printf("\033[%dm%s\033[0m",
            (int)(fmtSettings.textStyle),
            rune.get()
        );
    }
    else {
        bool withoutRGB = fmtSettings.fontRBG == nullptr && fmtSettings.backgoundRBG == nullptr;

        if (withoutRGB) {
            printf("\033[%d;%d;%dm%s\033[0m",
                (int)(fmtSettings.textStyle),
                int(fmtSettings.backgroundColor),
                (int)fmtSettings.fontColor,
                rune.get()
            );
        }
        else {
            printf("\033[%d;%d;%dm",
                (int)(fmtSettings.textStyle),
                int(fmtSettings.backgroundColor),
                (int)fmtSettings.fontColor
            );

            if (fmtSettings.backgoundRBG) {
                printf("\033[48;2;%d;%d;%dm",
                    fmtSettings.backgoundRBG->r,
                    fmtSettings.backgoundRBG->g,
                    fmtSettings.backgoundRBG->b
                );
            }

            if (fmtSettings.fontRBG) {
                printf("\033[38;2;%d;%d;%dm",
                    fmtSettings.fontRBG->r,
                    fmtSettings.fontRBG->g,
                    fmtSettings.backgoundRBG->b
                );
            }
            printf("%s\033[0m",
                rune.get()
            );
        }
    }

    // \033[53
    // \033[38;2;201;200;59m    полный спектр rbg для цвета и фона
    // \033[38;5;120            256 цветов для шрифта и фона


    // true color + text style (underline)
    //printf("\033[%dm", (int)(fmtSettings.textStyle));
    //printf("\033[38;2;201;200;59m"); // fontColor;
    //printf("%s\033[0m", rune.get());


    // \033[38;2;<r>;<g>;<b>m           #Select RGB foreground color
    // \033[48; 2; <r>; <g>; <b>m       #Select RGB background color
    //printf("\033[38;2;201;200;59\033[48;2;201;100;59m%s", data.get());

    //printf("\033[5;%d;%dm%s\033[0m", BackgroundColor::Red, FontColor::Yellow, data.get());
}

TerminalCell::TerminalCell(const Rune& rune) 
    : rune(rune)
{}

TerminalCell::TerminalCell(const Rune& rune, const FormatSettings* formatSettings)
    : rune(rune)
    , formatSettings(formatSettings)
{}
