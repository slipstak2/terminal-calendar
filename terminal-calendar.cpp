// terminal-calandar.4cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include "TerminalApplication.h"

#define IS_BIG_ENDIAN (*(WORD *)"\0\x2" == 0x200)

#define LOG(x) std::cout << (#x) << ": " << (x) << std::endl;

int main()
{
   
    system("chcp 65001");
    SetConsoleOutputCP(CP_UTF8);

    unsigned char I[4] = "И";
    LOG(I);
    // 208, 152, 0, 0  
    for (int i = 0; i < 4; ++i) {
        printf("%d ", I[i]);
    }
    printf("\n");
    unsigned char p[4] = "◀";
    // 226 151 128 0
    for (int i = 0; i < 4; ++i) {
        printf("%d ", p[i]);
    }
    printf("\n");
    int x = 9660;
    char check[4] = { 188, 37, 0, 0 };
    std::cout << "check: " << check << std::endl;

    // 1
    LOG(IS_BIG_ENDIAN);
   
    unsigned char* xx = (unsigned char*)&x;
    printf("%s\n", (char*)&x);
    printf((char*)p);
    TerminalApplication app;
    //app.Run();

    return 0;
}
