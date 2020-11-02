/*
Copyright (c) 2020  MIPT

Module Name:
    IRQ.c

Abstract:
    Реализует синхронные прерывания

Author:
    JulesIMF

Last Edit:
    16.10.2020 11:45

Edit Notes:

*/
#include "IRQ.h"
#include "../SetColor.h"
#include "../Common.h"

#define NEW_IRQ(name)       \
void IRQ_##name(Core* core) \
{                           \
    assert(core);           \
    setColor(LightRed);     \
    printf("Interruption: %s. Stop.\n", #name);\
    setColor(White);        \
    core->isWorking = 0;    \
}

NEW_IRQ(StackError)
NEW_IRQ(DivideByZero)
NEW_IRQ(InvalidCommand)
NEW_IRQ(InvalidParameters)
NEW_IRQ(HeapOverflow)
NEW_IRQ(TapeError)
