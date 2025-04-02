#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include "TerminalCalendarApplication.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    app().Run();

    //freopen("field.txt", "r", stdin);
    //std::string row;
    //while (getline(std::cin, row)) {
    //    std::cout << row << std::endl;
    //}

    return 0;
}
