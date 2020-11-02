/*
Copyright (c) 2020  MIPT

Module Name:
    Parsing.c

Abstract:
    Реализует функции парсинга

Author:
    JulesIMF

Last Edit:
    23.10.2020 1:03

Edit Notes:

*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "../ErrorMessage.h"
#include "Line.h"
#include "Parsing.h"
#include "Routine.h"
#include "../Common.h"

//****************************************************************
//* Символы метки, установки прерывания, установки размера стека *
//****************************************************************


#define STACK_SIZE      2000
#define FAILED_BYTE     ~'\0'



char const* isLabel(Line line)
{
    if (line.size > 1 && line.string[line.size - 1] == LABEL_SYMBOL)
        return line.string;

    else
        return NULL;
}

int isSameLabel(char const* label, char const* candidate)
{
    while (*label == *candidate && *label && *candidate)
        label++, candidate++;

    return (*label == '\0') && (*candidate == LABEL_SYMBOL);
}

unsigned long long isStackSet(Line line)
{
    if (*(line.string) == STACKSET_SYMBOL && *(line.string + 1))
    {
        unsigned long long newStackSize = 0;
        sscanf(line.string + 1, "%lld", &newStackSize);
        return newStackSize;
    }

    return 0;
}

long long isIntSet(Line line)
{
    long long returned = 0;
    int nInt = 0, address = 0;
    if (*(line.string) == INTSET_SYMBOL && *(line.string + 1))
    {
        sscanf(line.string + 1, "%d %d", &nInt, &address);
        returned = nInt;
        returned <<= 32;
        returned += address;
    }

    return returned;
}


typedef struct
{
    byte secondByte;
    long long value[2];
    int success;
} StdInstructionArgs;

typedef struct
{
    int isReference;
    int length;
    int success;
    long long value;
    byte AST;
} ParsedParameters;

int getRegister(char const* buffer, byte* AST)
{
    for (int i = 0; i != 4; i++)
    {
        if (buffer[0] == 'r' &&
            buffer[1] == 'a' + i &&
            buffer[2] == 'x')
        {
            *AST = AST_RAX + i;
            return 3;
        }
    }

    if (buffer[0] == 'r' &&
        buffer[1] == 's' &&
        buffer[2] == 'p')
    {
        *AST = AST_STACKTOP;
        return 3;
    }

    return 0;
}

int getConstant(char const* buffer, long long* value)
{
    static char const* formats[] =
    {
        "%lld%n",
        "%llx%n",
        "%lf%n",
    };

    int procN = 0;
    for (int i = 0; i != 3; i++)
    {
        if (sscanf(buffer, formats[i], value, &procN) > 0 && (isspace(buffer[procN]) || !buffer[procN]))
            return procN;
    }

    return 0;
}

ParsedParameters getDereferencing(char const* string)
{
    ParsedParameters returned;
    returned.success = 0;
    static char buffer[64];
    sscanf(string, "%63s", buffer);
    int length = strlen(buffer);
    if (buffer[0] != '[' || buffer[length - 1] != ']')
        return returned;

    returned.length = length;
    buffer[length - 1] = '\0';
    char* statement = buffer + 1;
    returned.AST = FAILED_BYTE;
    int procN = 0;

    procN = getRegister(statement, &returned.AST);
    if (procN)
        returned.success = 1,
        returned.isReference = 1,
        returned.value = 0ll;

    statement += procN;
    if (*statement == '+')
        statement++;
    if (getConstant(statement, &returned.value))
    {
        returned.success = 1,
            returned.isReference = 1;
        if (returned.AST == FAILED_BYTE)
            returned.AST = AST_CONST;
    }
    return returned;
}

ParsedParameters parseStdInsructionsArgsComponent(char const* string)
{
    int procN = 0;
    ParsedParameters parsedParameters = getDereferencing(string);
    if (parsedParameters.success)
        return parsedParameters;


    if (procN = getRegister(string, &parsedParameters.AST))
    {
        parsedParameters.success = 1;
        parsedParameters.isReference = 0;
        parsedParameters.length = procN;
        parsedParameters.value = 0;
        return parsedParameters;
    }

    if (procN = getConstant(string, &parsedParameters.value))
    {
        parsedParameters.success = 1;
        parsedParameters.isReference = 0;
        parsedParameters.length = procN;
        parsedParameters.AST = AST_CONST;
        return parsedParameters;
    }

    return parsedParameters;
}

static StdInstructionArgs parseStdInstructionArgs(char const* string, int nParameters, size_t lineN)
{
    StdInstructionArgs stdInstructionArgs = { 0, 0, 0, 0 };
    
    for (int i = 0; i != nParameters; i++)
    {
        ParsedParameters parsedParameters = parseStdInsructionsArgsComponent(string);
        if (!parsedParameters.success)
        {
            errorMessage("can`t recognize parameter", lineN);
            return stdInstructionArgs;
        }
        stdInstructionArgs.value[i] = parsedParameters.value;
        stdInstructionArgs.secondByte |=
            ( ((parsedParameters.AST << 1) | parsedParameters.isReference) << (4 * i) );

        string += parsedParameters.length;
        while (isspace(*string)) string++;
    }

    stdInstructionArgs.success = 1;

    return stdInstructionArgs;
}

Instruction getInstruction(Line line, size_t lineN, Label* labels, size_t nLabels)
{
    static char mnemonic[10] = "";
    Instruction instruction = { 0, { 0 } };
    if (isLabel(line))
        return instruction;

    int shift = 0;
    sscanf(line.string, "%10s%n", mnemonic, &shift);

    //
    //***********************************************************
    //*************************SPECIALS**************************
    //***********************************************************
    //

    //out
    if (!strcmp("out", mnemonic))
    {
        instruction.code[instruction.length++] = cmd_out;
        char mode[5] = { 0, 0, 0, 0, 0 };
        sscanf(line.string + shift + 1, "%3s", mode);
        byte secondByte = OUTF_LLD;
        if (!strcmp("llx", mode))
            secondByte = OUTF_016LLX;

        if (!strcmp("lf", mode))
            secondByte = OUTF_LF;

        instruction.code[instruction.length++] = secondByte;
        return instruction;
    }


    //inc dec
    if (!strcmp("inc", mnemonic) || !strcmp("dec", mnemonic))
    {

        instruction.code[instruction.length++] = 
            ( (mnemonic[0] == 'i') ? cmd_inc : cmd_dec );

        char reg[4] = "";
        if (!sscanf(line.string + 1 + shift, "%3s", reg))
        {
            errorMessage("can`t recognize register", lineN);
            instruction.length = 0;
            return instruction;
        }

        int AST = 0;
        if (getRegister(reg, &AST) && AST < AST_STACKTOP)
        {
            instruction.code[instruction.length++] = AST;
            return instruction;
        }

        errorMessage("unknown register", lineN);

        instruction.length = 0;
        return instruction;
    }



    //***********************************************************
    //*************************DEFINES***************************
    //***********************************************************





#define REJECT_SPECIALS
#define REJECT_STANDARD
#define UNWRAP_CMD(name, nArg, body)                                                                            \
    if (!strcmp(#name, mnemonic))                                                                               \
    {                                                                                                           \
        instruction.code[instruction.length++] = cmd_##name;                                                    \
        char labelname[20] = "";                                                                                \
        sscanf(line.string + shift + 1, "%20s", labelname);                                                     \
        for(int i = 0; i != nLabels; i++)                                                                       \
        {                                                                                                       \
            if(isSameLabel(labelname, labels[i].label))                                                         \
            {                                                                                                   \
                *(int*)(&instruction.code[instruction.length]) = labels[i].address; instruction.length += 4;    \
                return instruction;                                                                             \
            }                                                                                                   \
        } errorMessage("no such label", lineN);                                                                 \
    instruction.length = 0; return instruction;                                                                 \
    }

#include "../Commands definitions.h"

#undef  REJECT_STANDARD
#undef  UNWRAP_CMD
#define REJECT_JMP
    
#define UNWRAP_CMD(name, nArg, body)                                                                \
if (!strcmp(#name, mnemonic))                                                                       \
{                                                                                                   \
    instruction.code[instruction.length++] = cmd_##name;                                            \
    StdInstructionArgs args = parseStdInstructionArgs(line.string + shift + 1, nArg, lineN);        \
    if (!args.success)                                                                              \
    {                                                                                               \
        instruction.length = 0;                                                                     \
        return instruction;                                                                         \
    }                                                                                               \
    if(nArg)                                                                                        \
    {                                                                                               \
    instruction.code[instruction.length++] = args.secondByte;                                       \
    *(long long*)(&instruction.code[instruction.length]) = args.value[0];                           \
    instruction.length += sizeof(long long);                                                        \
    }                                                                                               \
    if(nArg == 2)                                                                                   \
    {                                                                                               \
    *(long long*)(&instruction.code[instruction.length]) = args.value[1];                           \
    instruction.length += sizeof(long long);                                                        \
    }                                                                                               \
    return instruction;                                                                             \
}

#include "..\Commands definitions.h"

    errorMessage("unknown mnemonic", lineN);

    instruction.length = 0;
    return instruction;
}

void setLabels(ProgramParameters* programParameters, Line* lines, int nLines)
{
    for (int i = 0; i != nLines; i++)
        programParameters->nLabels += !!isLabel(lines[i]);

    programParameters->labels = (Label*)calloc(programParameters->nLabels, sizeof(Label));
    {
        size_t labelsPtr = 0;
        for (int i = 0; i != nLines; i++)
        {
            char const* label = isLabel(lines[i]);
            if (label)
            {
                programParameters->labels[labelsPtr].label = label;
                programParameters->labels[labelsPtr].address = -1;
                labelsPtr++;
            }
        }
    }
}

static int proceedSpecial(
    int i,
    ProgramParameters* programParameters, 
    Line* lines, 
    int nLines,
    int* rip, 
    int* isDataMode)

{
    static int labelsPtr            = 0;
    static char* newLabel           = NULL;
    static long long newStackSize   = 0;
    static long long newIrq         = 0;
    if (!lines[i].size)
        return 1;

    if (lines[i].string[0] == DATA_SYMBOL)
    {
        *rip += lines[i].size - 1;
        if (!(*isDataMode))
            *(isDataMode) = 1,
            (*rip)++;
        return 2;
    }

    if (newLabel = isLabel(lines[i]))
    {
        programParameters->labels[labelsPtr++].address = *rip;
        if (i + 1 != nLines && !(*isDataMode) && lines[i + 1].string[0] == DATA_SYMBOL)
            programParameters->labels[labelsPtr - 1].address++;

        return 3;
    }

    if (newStackSize = isStackSet(lines[i]))
    {
        programParameters->stackSize = newStackSize;
        return 4;
    }

    return 0;
}

ProgramParameters getProgramParameters(Line* lines, size_t nLines)
{
    assert(lines);
    ProgramParameters programParameters;

    //
    //Инициализация
    //
    programParameters.stackSize = STACK_SIZE;
    programParameters.nLabels = 0;
    for (int i = 0; i != 8; i++)
        programParameters.IRQ[i] = -1;

    setLabels(&programParameters, lines, nLines);


    int rip = 0;
    int isDataMode = 0;
    Instruction instruction;

    for (int i = 0; i != nLines; i++)
    {
        if (proceedSpecial(
            i,
            &programParameters,
            lines,
            nLines,
            &rip,
            &isDataMode
        ))
        {
            continue;
        }

        if (isDataMode ||
            (instruction = getInstruction(lines[i], 
                                           i + 1, 
                                           programParameters.labels, 
                                           programParameters.nLabels
                                         ),
             !instruction.length))
        {
            if(isDataMode)
                errorMessage("unexpected instruction in data mode", i + 1);

            free(programParameters.labels);
            programParameters.labels = NULL;
            programParameters.programSize = -1; //Программа некорректна
            return programParameters;
        }

        rip += instruction.length;
    }

    programParameters.programSize = rip;
    return programParameters;
}
