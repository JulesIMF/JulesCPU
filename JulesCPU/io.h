#include "Core.h"
#ifndef _JCPU_IO
#define _JCPU_IO
void cpu_printf(Core* core, char const* src);
void cpu_cls(Core* core);
int GetConsoleCursorPosition(Core* core);
void cpu_PrnBuf(Core* core);
void GotoXY(int x, int y);
void cpu_normalize(Core* core);
int cpu_getConsoleCursorPosition(Core* core);
#endif