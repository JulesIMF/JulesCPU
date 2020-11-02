/*
Copyright (c) 2020  MIPT

Module Name:
    Execution (auxiliary).с

Abstract:
    Реализует вспомогательный интерфейс исполнения

Author:
    JulesIMF

Last Edit:
    19.10.2020 0:14

Edit Notes:

*/
#define _CRT_SECURE_NO_WARNINGS
#include "Execution.h"
#include "../Common.h"
#include "../Hash.h"
#include "../Macrofnc.h"

Parameters getParameters(Core* core, int nParameters)
{
    assert(core);
    Parameters parameters;
    parameters.isValidParameters = parameters.first = parameters.second = 0;
    if (nParameters == 0)
    {
        parameters.isValidParameters = 1;
        return parameters;
    }
    long long value[2] = { 0,0 };


    SecondByte secondByte = getSecondByte(core);
    if (secondByte.isPtr_1 == -1)
        return parameters;

    /*
        Argument signature:
            0:      isPtr_1
            1 2 3:  AST_1
            4:      isPtr_2
            5 6 7:  AST_2


        isPtr   - флаг, показывающий, необходимо ли разыменовывать указатель
        AST     - Argument signature type
    */

    int isPtr[2] = { secondByte.isPtr_1,  secondByte.isPtr_2 };
    int AST  [2] = { secondByte.AST_1, secondByte.AST_2 };

    

    /*
        Argument signature type:
            0: rax
            1: rbx
            2: rcx
            3: rdx
            4: stackTop
            5: const
            6:
            7:
    */

    if (AST[0] > 5 || AST[1] > 5)
    {
        IRQ_InvalidCommand(core);
        return parameters;
    }


    for (int iValue = 0; iValue != nParameters; iValue++)
    {
        IS_TAPE_END(core, long long, parameters);
        switch (AST[iValue])
        {
        case AST_RAX:
        case AST_RBX:
        case AST_RCX:
        case AST_RDX:
            value[iValue] = core->rx[AST[iValue]];
            break;

        case AST_STACKTOP:
            STACK_TOP(core, value[iValue], parameters);
            break;

        case AST_CONST:
            break;

        default:
            IRQ_InvalidCommand(core);
            return parameters;
        }

        value[iValue] += GET_ARGUMENT(core, long long);

        if (isPtr[iValue])
        {
            if (value[iValue] >= core->capacity)
            {
                IRQ_HeapOverflow(core);
                return parameters;
            }
            value[iValue] = core->ram[value[iValue]];
        }
    }


    if (AST[0] == 4 && AST[1] == 4 && nParameters == 2)
    {
        long long tmp = value[0];
        value[0] = value[1];
        value[1] = tmp;
    }

    parameters.first  = value[0];
    parameters.second = value[1];
    parameters.isValidParameters = 1;
    return parameters;
}

int popBySecondByte(Core* core, SecondByte secondByte)
{
    long long value = GET_ARGUMENT(core, long long);
    if (secondByte.AST_1 > AST_CONST || (secondByte.AST_1 == AST_CONST && !secondByte.isPtr_1))
        return -1;

    if (secondByte.isPtr_1)
    {
        long long address = 0;
        if (secondByte.AST_1 < AST_STACKTOP)
            address = core->rx[secondByte.AST_1];
        if (secondByte.AST_1 == AST_STACKTOP)
            STACK_TOP(core, address, -1);

        address += value;
        if (address >= core->capacity)
        {
            IRQ_TapeError(core);
            return -1;
        }
        STACK_POP(core, core->ram[address], -1);
        return 0;
    }

    if (secondByte.AST_1 == AST_CONST)
        return -1;

    STACK_POP(core, value, -1);
    if (secondByte.AST_1 == AST_STACKTOP)
        return 0;

    
    
    core->rx[secondByte.AST_1] = value;
    return 0;
}


int isTapeEnd(Core* core, size_t size)
{
    assert(core);
    return core->rip + size > core->program.size;
}

int programShift(byte* byteCode, size_t byteCodeSize, Core* core)
{
    if (byteCodeSize < sizeof(jbcSignature))
        return -1;

    if (*(int*)byteCode != 'EXEJ')
        return -1;

    byteCode += sizeof(int);

    if (*(int*)byteCode < _JCPU_MIN_ASM_VERSION)
        return -1;

    byteCode += sizeof(int);

    long long hash = getHash(byteCode + _JCPU_SIGNATURE_SIZE - 8, byteCodeSize - _JCPU_SIGNATURE_SIZE);
    /*if (hash != *(long long*)byteCode)
        return -1;*/

    byteCode += sizeof(long long);
    //core->capacity = *(long long*)byteCode;
    core->coreStack = stackNew(*(long long*)byteCode);
    //core->ram = (long long*)calloc(core->capacity, sizeof(long long));

    return sizeof(jbcSignature);
}

Program getProgram(byte* byteCode, size_t byteCodeSize, Core* core)
{
    assert(byteCode);
    int shift = programShift(byteCode, byteCodeSize, core);
    Program program;
    if (shift == -1)
        program.tape = NULL, program.size = 0;
    else
        program.tape = byteCode + shift, program.size = byteCodeSize - shift;

    return program;
}

/*
static void IRQ_StackError(Core* core);         //0
static void IRQ_DivideByZero(Core* core);       //1
static void IRQ_InvalidCommand(Core* core);     //2
static void IRQ_InvalidParameters(Core* core);  //3
static void IRQ_InvalidIRQ(Core* core);         //4
*/
void proceedInterruption(Core* core)
{
    assert(core);
}

SecondByte getSecondByte(Core* core)
{
    assert(core);
    SecondByte secondByte_;
    if (isTapeEnd(core, 1))
    {
        {
            IRQ_InvalidParameters(core);
            secondByte_.isPtr_1 = -1;
            return secondByte_;
        }
    }

    byte secondByte = GET_ARGUMENT(core, byte);
    secondByte_.isPtr_1 = secondByte & (1 << 0);
    secondByte_.isPtr_2 = secondByte & (1 << 4);
    secondByte_.AST_1 = (secondByte & (7 << 1)) >> 1;
    secondByte_.AST_2 = (secondByte & (7 << 5)) >> 5;
    return secondByte_;
}
