#ifndef _JCPU_SETCOLOR
#define _JCPU_SETCOLOR
#include <Windows.h>
static void setColor(int text)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)(text));
}
#endif