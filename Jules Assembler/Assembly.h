/*
Copyright (c) 2020  MIPT

Module Name:
    Assembly.h

Abstract:


Author:
    JulesIMF

Last Edit:
    22.10.2020

Edit Notes:

*/

#ifndef _JCPU_ASSEMBLY
#define _JCPU_ASSEMBLY
#include <stdlib.h>
#include "Line.h"
#include "Routine.h"

typedef struct
{
    byte* code;
    size_t programSize;
} ByteCode;


ByteCode getByteCode(Line* lines, size_t nLines, char const* asmName);

void prepareLinesToAssembly(Line* lines, int nStrings);
#endif