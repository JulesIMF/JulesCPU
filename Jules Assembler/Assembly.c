/*
Copyright (c) 2020  MIPT

Module Name:
    Assembly.c

Abstract:


Author:
    JulesIMF

Last Edit:
    22.10.2020

Edit Notes:

*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Assembly.h"
#include "Parsing.h"
#include "../Common.h"
#include "../Hash.h" 
#include "Files.h"

int proceedControlSymbol(char const* string, int* i, int size)
{
    static char escape       [] = { 't',  'n',  'r',  '0',  'f',  '\\', 'b',  'a' };
    static char controlSymbol[] = { '\t', '\n', '\r', '\0', '\f', '\\', '\b', '\a' };

    if (string[*i] == '\\')
    {
        (*i)++;
        if (*i == size)
            return -1;

        for (int jEsc = 0; jEsc != sizeof(escape); jEsc++)
            if (string[*i] == escape[jEsc])
                return controlSymbol[jEsc];
    }
    return -1;
}

void translateData(ByteCode* code, int* pRealRip, Line line)
{
    int realRip = *pRealRip;
    int size = line.size - 1;
    char const* string = line.string + 1;

    

    for (int i = 0; i != size; i++)
    {
        //escape
        int control = proceedControlSymbol(string, &i, size);
        if (control != -1)
            code->code[realRip++] = control;
        else
            code->code[realRip++] = string[i];
    }

    *pRealRip = realRip;
}

void writeSignature(ByteCode* bytecode, ProgramParameters parameters)
{
    jbcSignature signature;
    signature.signature = 'EXEJ';
    signature.stackSize = parameters.stackSize;
    for (int i = 0; i != 8; i++)
        signature.INT[i] = parameters.IRQ[i];

    signature.asmVersion = _JCPU_ASM_VERSION;
    signature.checksum = getHash(bytecode->code + _JCPU_SIGNATURE_SIZE, bytecode->programSize - _JCPU_SIGNATURE_SIZE);
    assert(bytecode->code);
    *(jbcSignature*)(bytecode->code) = signature;
}

static void putToListing(FILE* file, int realRip, Instruction instruction, Line line)
{
    fprintf(file, "0x%08X  (0x%08X)\t%-20s\t", realRip, (int)(realRip - _JCPU_SIGNATURE_SIZE), line.string);
    for (int i = 0; i != instruction.length; i++)
    {
        fprintf(file, "%02X ", instruction.code[i] & 255);
    }

    fprintf(file, "\n");
}


static FILE* initListing(char const* asmName, ProgramParameters parameters)
{
    int asmName_size = strlen(asmName);
    char const* addName = " - listing.txt";
    char* listingName = (char*)calloc(asmName_size + 17, sizeof(char));
    strcpy(listingName, asmName);
    strcat(listingName, addName);
    FILE* file = fopen(listingName, "w");
    fprintf(file, "Listing file for \"%s\" assembly\n\n", asmName);

    fprintf(file, "Program size = %lld\nStack size   = %lld\n", parameters.programSize, parameters.stackSize);
    fprintf(file,
        "\n\n\n--------------------------------------------------------------------------------------------------------------\n"
        "real RIP    virtual RIP         mnemonics               byte code\n"
        "--------------------------------------------------------------------------------------------------------------\n");

    return file;
}

static int proceedSpecial(ByteCode* byteCode, Line line, int* dataMet, FILE* listingFile, int* realRip)
{
    if (isLabel(line))
    {
        if (!(*dataMet))
            fprintf(listingFile, "%s\n", line.string);
        return 1;
    }
    if (
        !line.size      ||
        isIntSet(line)  ||
        isStackSet(line)
        )
    {
        return 1;
    }

    if (line.string[0] == DATA_SYMBOL)
    {
        if (!(*dataMet))
            byteCode->code[(*realRip)++] = DATA_SYMBOL, *dataMet = 1;

        translateData(byteCode, realRip, line);
        return 1;
    }
    return 0;
}

ByteCode getByteCode(Line* lines, size_t nLines, char const* asmName)
{
    ByteCode byteCode = { NULL, 0 };
    ProgramParameters parameters = getProgramParameters(lines, nLines);
    if (parameters.programSize == -1)
        return byteCode;

    byteCode.code = (byte*)calloc(_JCPU_SIGNATURE_SIZE + parameters.programSize + 2, sizeof(byte));
    FILE* listingFile = initListing(asmName, parameters);
    int realRip = _JCPU_SIGNATURE_SIZE;
    int dataMet = 0;

    for (int i = 0; i != nLines; i++)
    {
        if (proceedSpecial(&byteCode, lines[i], &dataMet, listingFile, &realRip))
            continue;

        Instruction instruction =
            getInstruction(lines[i], i + 1, parameters.labels, parameters.nLabels);

        if (!instruction.length)
        {
            byteCode.programSize = 0;
            return byteCode;
        }

        putToListing(listingFile, realRip, instruction, lines[i]);
        for (int i = 0; i != instruction.length; i++)
            assert(realRip < _JCPU_SIGNATURE_SIZE + parameters.programSize),
            byteCode.code[realRip++] = instruction.code[i];
    }

    byteCode.programSize = realRip;
    writeSignature(&byteCode, parameters);
    return byteCode;
}


void prepareLinesToAssembly(Line* lines, int nStrings)
{
    for (size_t i = 0; i != nStrings; i++)
        if (lines[i].string[0] != DATA_SYMBOL)
            replaceSemicolon(lines + i);

    eraseLeadingSpaces(lines, nStrings);

    for (size_t i = 0; i != nStrings; i++)
        if (lines[i].string[0] != DATA_SYMBOL)
            turnToLowercase(lines + i);
}
