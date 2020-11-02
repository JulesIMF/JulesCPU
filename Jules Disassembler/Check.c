/*
Copyright (c) 2020  MIPT

Module Name:
    Disassembling.c

Abstract:
    Разбирает программу

Author:
    JulesIMF

Last Edit:
    26.10.2020

Edit Notes:

*/

#include <stdlib.h>
#include <stdio.h>
#include "../Common.h"
#include "Includes.h"
#include "../Hash.h"

long long isCorrect(byte* byteCode, size_t byteCodeSize)
{
    if (byteCodeSize < sizeof(jbcSignature))
        return 0;

    if (*(int*)byteCode != 'EXEJ')
        return 0;

    byteCode += sizeof(int);

    if (*(int*)byteCode < _JCPU_MIN_ASM_VERSION)
        return 0;

    byteCode += sizeof(int);

    long long hash = getHash(byteCode + _JCPU_SIGNATURE_SIZE - 8, byteCodeSize - _JCPU_SIGNATURE_SIZE);
    if (hash != *(long long*)byteCode)
        return 0;


    byteCode += sizeof(long long);
    return *(long long*)byteCode;
}
