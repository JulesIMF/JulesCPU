/*
Copyright (c) 2020  MIPT

Module Name:
    Core.h

Abstract:
    Реализует класс ядра процессора

Author:
    JulesIMF

Last Edit:
    16.10.2020 10:08

Edit Notes:

*/

#include "Core.h"


Core* newCore(size_t ramCapacity, int videoX, int videoY)
{
    Core* core = (Core*)calloc(1, sizeof(Core));
    assert(core);
    core->flags = FLAG_IF;
    core->isWorking = 1;
    core->rip = 0;
    core->interruption = -1;
    core->videoX = videoX;
    core->videoY = videoY;
    //core->coreStack = stackNew(capacity);
    core->capacity = ramCapacity + videoX * videoY;
    core->ram = (long long*)calloc(core->capacity, sizeof(long long));
    return core;
}

void  deleteCore(Core* core)
{
    if (core == NULL)
        return;
    stackDelete(core->coreStack);
    free(core->ram);
    free(core);
}

int setProgram(Core* core, Program program)
{
    assert(core);
    core->program = program;
    return program.tape == NULL;
}
