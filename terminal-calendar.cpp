// terminal-calandar.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include "TerminalApplication.h"

int main()
{
    //system("chcp 65001");
    SetConsoleOutputCP(CP_UTF8);

    TerminalApplication app;
    app.Run();

    return 0;
}
