#include <iostream>
#include <Windows.h>
#include "DemoTerminalApplication.h"

int main()
{
    //system("chcp 65001");
    SetConsoleOutputCP(CP_UTF8);

    DemoTerminalApplication app;
    app.Run();

    return 0;
}
