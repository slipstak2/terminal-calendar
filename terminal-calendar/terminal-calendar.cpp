#include <iostream>
#include <Windows.h>
#include "TerminalCalendarApplication.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    TerminalCalendarApplication app;
    app.Run();

    return 0;
}
