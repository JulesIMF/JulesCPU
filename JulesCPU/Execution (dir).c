/*
Copyright (c) 2020  MIPT

Module Name:
    Execution (direct).�

Abstract:
    ��������� ���������������� ��������� ����������

Author:
    JulesIMF

Last Edit:
    19.10.2020 0:14

Edit Notes:

*/
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include "..\ErrorMessage.h"
#include "IRQ.h"
#include "Execution.h"
#include "..\Common.h"
#include "..\Macrofnc.h"
#include "io.h"


/**
 * ��������� ���������. ����� �������� ��������� �� �������
 *
 * \param core
 * \param translatedFile
 * \param fileSize
 * \return 0 ���� ��������� �������� ��� ������, 1 �����
 */
int runProgram(Core* core, byte* translatedFile, int fileSize)
{
    assert(core);
    assert(translatedFile);
    Program program = getProgram(translatedFile, fileSize, core);

    if (program.tape == NULL)
    {
        errorMessage("program is damaged or has too old version", 0);
        return 1;
    }

    core->program = program;
    core->rip = 0;
    core->interruption = -1;
    //TODO: intTable
    core->rx[0] = core->rx[1] = core->rx[2] = core->rx[3] = 0ll;
    core->flags = FLAG_IF;
    for (int i = 0; i != core->videoY; i++)
        _putch('\n');

    cpu_getConsoleCursorPosition(core);
    GotoXY(core->coord.X, core->coord.Y);
    return 0;
}


#define UNWRAP_CMD(name, args, body)                    \
case cmd_##name:                                        \
/*printf("%s\n", #name); */                             \
parameters = getParameters(core, args);                 \
if(parameters.isValidParameters == 0) return -1;        \
body                                                    \
return opcode;


/**
 * .
 *
 * \param core
 * \return -1 ���� ��������� ������ �� ����� �����������, opcode �����
 */
int executeNextCommand(Core* core)
{
    if (core->rip >= core->program.size
        ||
        core->isWorking == 0)
    {
        return -1;
    }

    byte opcode = 0;

    Parameters parameters;

    switch (opcode = GET_ARGUMENT(core, byte))
    {

    #include "..\Commands definitions.h"
    
    case DATA_SYMBOL:
        return -1;
    

    default:
        printf("UNKNOWN COMMAND - %02x\n", opcode);
        IRQ_InvalidCommand(core);
        return -1;
    }
}
