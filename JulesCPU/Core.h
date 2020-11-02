/*
Copyright (c) 2020  MIPT

Module Name:
    Core.h

Abstract:
    Описывает класс ядра процессора

Author:
    JulesIMF

Last Edit:
    16.10.2020 10:08

Edit Notes:

*/

#ifndef _JCPU_CORE
#define _JCPU_CORE
#include <Windows.h>
#include "Stack.h"
#include <assert.h>

/*
            Регистры флагов

    00  СF      Carry flag
    01
    02  PF      Partity flag
    03
    04  AF      Auxiliary Carry Flag
    05
    06  ZF      Zero flag
    07  SF      Sign flag
    08  TF      Trap flag
    09  IF      Interruption enable flag
    10  DF      Direction flag
    11  OF      Overflow flag
    12  IOPL
    13  IOPL
    14  NT      Nested task
    15
*/

static const int
FLAG_CF = (short)(1 << 0),
FLAG_PF = (short)(1 << 2),
FLAG_AF = (short)(1 << 4),
FLAG_ZF = (short)(1 << 6),
FLAG_SF = (short)(1 << 7),
FLAG_TF = (short)(1 << 8),
FLAG_IF = (short)(1 << 9),
FLAG_DF = (short)(1 << 10),
FLAG_OF = (short)(1 << 11),
FLAG_NT = (short)(1 << 14);

typedef struct _Core Core;
typedef char byte;

typedef struct
{
    byte* tape;
    size_t size;
} Program;

struct _Core
{
    int isWorking;
    int interruption;
    long long* ram;
    long long capacity;
    int videoX, videoY;
    int currentX, currentY;
    COORD coord;

    //Таблица обработчиков асинхронных прерываний
    int intTable[8];

    long long rx[4];

    unsigned short int flags;
    Stack* coreStack;
    unsigned int rip;
    Program program;
};

Core* newCore(size_t ramCapacity, int videoX, int videoY);
void  deleteCore(Core* core);

/**
 *\return 0 если установление прошло успешно, 1 если программа не прошла проверку на корректность
 */
int setProgram(Core* core, Program program);

#endif // !_JCPU_CORE
