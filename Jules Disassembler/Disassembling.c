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

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../Common.h"
#include "Includes.h"
#include "../ErrorMessage.h"

//typedef struct
//{
//    int  nLabels;
//    int* labels;
//    int  nCommands;
//} DisassemblyParameters;


int cmp(void const* first, void const* second)
{
    return *(int*)first - *(int*)second;
}

int isCorrectProgram(DisassemblyParameters* parameters, byte* byteCode, size_t size)
{
    int rip = 0;
    while (rip < size)
    {
        parameters->nCommands++;
        switch (byteCode[rip])
        {
        case DATA_SYMBOL:
            rip = size;
            break;

        case cmd_inc:
        case cmd_dec:
        case cmd_out:
            rip += 2;
            continue;


        #define UNWRAP_CMD(name, nArgs, body) case cmd_ ## name: rip += 5; parameters->nLabels++; continue;
        #define REJECT_SPECIAL
        #define REJECT_STANDARD
        #include "../Commands definitions.h"
        #undef REJECT_STANDARD
        #undef UNWRAP_CMD
        #define REJECT_JMP
        #define UNWRAP_CMD(name, nArgs, body) case cmd_ ## name: rip += 1 + 8 * (nArgs) + (!!nArgs); continue;
        #include "../Commands definitions.h"

        default:
            parameters->nCommands = -1;
            errorMessage("unknown instruction", 0);
            return 0;    
        }
    }

    if (rip != size)
    {
        parameters->nCommands = -1;
        printf("unexpected end of file");
        return parameters;
    }

    parameters->labels = (int*)calloc(parameters->nLabels, sizeof(int));
    return 1;
}

DisassemblyParameters getDisassemblyParameters(byte* byteCode, size_t size)
{
    DisassemblyParameters parameters = { 0, (int*)NULL, 0 };
    int rip = 0;

    if (!isCorrectProgram(&parameters, byteCode, size))
        return parameters;
    
    int labelsIndex = 0;

    while (rip != size && labelsIndex != parameters.nLabels)
    {
        switch (byteCode[rip])
        {
        case DATA_SYMBOL:
            break;
        case cmd_inc:
        case cmd_dec:
        case cmd_out:
            rip += 2;
            continue;

        #include "../Commands definitions.h"

        default:
            rip++;
            parameters.labels[labelsIndex++] = *(int*)(byteCode + rip);
            rip += 4;
        }
    }

    qsort(parameters.labels, parameters.nLabels, sizeof(int), cmp);
#undef UNWRAP_CMD
    return parameters;
}

void writeAlu(FILE* file, byte* code, int operands)
{
    if (!operands)
    {
        fprintf(file, "\n");
        return;
    }

    byte secondByte = *(code++);

    for (int i = 0; i != operands; i++)
    {
        long long parameter = *(long long*)code;
        code += sizeof(long long);
        int isPtr = secondByte & 1,
            AST = secondByte & (7 << 1);
        AST >>= 1;
        if (isPtr)
            fprintf(file, "[");

        if (AST < AST_STACKTOP)
            fprintf(file, "r%cx", 'a' + AST);

        if (AST == AST_STACKTOP)
            fprintf(file, "rsp");

        if (AST > AST_CONST)
            fprintf(file, "AST%d ", AST);
        
        if(AST == AST_CONST || isPtr)
            fprintf(file, "+0x%llX", parameter);

        if (isPtr)
            fprintf(file, "]");

        fprintf(file, " ");
        secondByte >>= 4;
    }
    fprintf(file, "\n");
}



FILE* getOutputFile(char const* fileName, int stackSize)
{
    int asmName_size = strlen(fileName);
    char const* addName = " - disassembler.asm";
    char* asmName = (char*)calloc(asmName_size + 21, sizeof(char));
    strcpy(asmName, fileName);
    strcat(asmName, addName);
    //printf("DEBUG: %s", asmName);
    FILE* file = fopen(asmName, "w");
    free(asmName);
    if (file == NULL)
    {
        errorMessage("failed to open output file", 0);
        return NULL;
    }

    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    fprintf(file,
        ";\n;JulesIMF disassemler, ver. %d\n;\n"
        ";Created:\t%s"
        ";Binary: \t\"%s\"\n;\n"
        ";Set stack size\n",
        _JCPU_ASM_VERSION,
        asctime(timeinfo),
        fileName);

    fprintf(file, "@%lld\n\n\n", stackSize);
    return file;
}

void proceedDataSection(
    byte* byteCode,
    int size,
    int rip,
    FILE* file,
    int nextLabel,
    DisassemblyParameters parameters);

void disassembly(byte* byteCode, size_t size, char const* fileName)
{
    //Проверка на корректность
    long long stackSize = isCorrect(byteCode, size);

    if (!stackSize)
    {
        errorMessage("program is damaged or has too old version", 0);
        return;
    }

    //Параметры
    DisassemblyParameters parameters = getDisassemblyParameters(byteCode + _JCPU_SIGNATURE_SIZE, size - _JCPU_SIGNATURE_SIZE);
    if (parameters.nCommands == -1)
    {
        return;
    }

    FILE* file = getOutputFile(fileName, stackSize);
    if (file == NULL)
        return;

    int rip = 0;
    int nextLabel = 0;
    byteCode += _JCPU_SIGNATURE_SIZE;
    size -= _JCPU_SIGNATURE_SIZE;
    if (parameters.nLabels)
        fprintf(file, "ENTRY_POINT:\n");

    while (rip < size)
    {
        //Пилим метки
        if (nextLabel < parameters.nLabels &&
            rip == parameters.labels[nextLabel])
        {
            fprintf(file, "\nLABEL_0x%X:\n", rip);
            nextLabel++;
            while (nextLabel != parameters.nLabels &&
                parameters.labels[nextLabel] == parameters.labels[nextLabel - 1]) nextLabel++;
        }


        //Команды
        if (parameters.nLabels)
            fprintf(file, "\t");

        switch (byteCode[rip])
        {
        case DATA_SYMBOL:
            break;

        case cmd_inc:
            rip++;
            fprintf(file, "inc r%cx\n", 'a' + (byteCode[rip++] & 3));
            continue;

        case cmd_dec:
            rip++;
            fprintf(file, "dec r%cx\n", 'a' + (byteCode[rip++] & 3));
            continue;

        case cmd_out:
            rip++;
            fprintf(file, "out");
            if (byteCode[rip] == OUTF_016LLX)
            {
                fprintf(file, " llx\n");
                rip++;
                continue;
            }

            if (byteCode[rip] == OUTF_LF)
            {
                fprintf(file, " lf\n");
                rip++;
                continue;
            }

            fprintf(file, "\n");
            rip++;
            continue;
        #define UNWRAP_CMD(name, operands, body) case cmd_ ## name: rip++; fprintf(file, "%s ", #name); \
            writeAlu(file, byteCode + rip, operands); rip += (!!operands) + 8 * (operands); continue;

        #include "../Commands definitions.h"
        #define REJECT_STANDARD
        #undef REJECT_JMP
        #undef UNWRAP_CMD
        #define UNWRAP_CMD(name, operands, body) case cmd_ ## name: rip++; fprintf(file, "%s LABEL_0x%X\n", #name, *(int*)(byteCode + rip)); rip += 4; continue;
        #include "../Commands definitions.h"
        }

        if (byteCode[rip] == DATA_SYMBOL)
        {
            rip++;
            break;
        }
    }

    proceedDataSection(byteCode, size, rip, file, nextLabel, parameters);
}



void putCharInEscapeForm(FILE* file, char character)
{
    static char const escape       [] = { 't',  'n',  'r',  '0',  'f',  '\\', 'b',  'a' };
    static char const controlSymbol[] = { '\t', '\n', '\r', '\0', '\f', '\\', '\b', '\a' };

    for (int i = 0; i != sizeof(escape); i++)
        if (character == controlSymbol[i])
        {
            fprintf(file, "\\%c", escape[i]);
            return;
        }

    fprintf(file, "%c", character);
}


void proceedDataSection(
    byte*                   byteCode, 
    int                     size, 
    int                     rip,
    FILE*                   file,
    int                     nextLabel,
    DisassemblyParameters   parameters)
{
    if (nextLabel < parameters.nLabels &&
        rip != parameters.labels[nextLabel])
    {
        fprintf(file, "~");
    }

    while (rip != size)
    {
        if (nextLabel < parameters.nLabels &&
            rip == parameters.labels[nextLabel])
        {
            fprintf(file, "\nLABEL_0x%X:\n~", rip);
            nextLabel++;
            while (nextLabel != parameters.nLabels &&
                   parameters.labels[nextLabel] == parameters.labels[nextLabel - 1]) nextLabel++;
        }

        putCharInEscapeForm(file, byteCode[rip++]);
    }
}


