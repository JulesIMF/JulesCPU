/*
Copyright (c) 2020  MIPT

Module Name:
    Parsing.c

Abstract:
    Реализует функции парсинга

Author:
    JulesIMF

Last Edit:
    22.10.2020 1:42

Edit Notes:

*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "Files.h"
#include "Routine.h"
#include "Assembly.h"
#include "Parsing.h"
#include "../Common.h"
#include "../ErrorMessage.h"



int main(int argc, char const** argv)
{
    if (argc == 1)
    {
        errorMessage("argc == 1", 0);
        return 1;
        //DEBUG
        argv[1] = "equation.jbc - disassembler.asm";
    }
    int nStrings = 0;
    Line* lines = NULL;
    int returnCode = getLines(argv[1], &lines, &nStrings);
    if (returnCode)
        return returnCode;

    prepareLinesToAssembly(lines, nStrings);


    ByteCode code = getByteCode(lines, nStrings, argv[1]);

    if (code.programSize)
    {
        char* programFileName = getProgramFileName(argv[1]);
        if (programFileName == NULL)
        {
            errorMessage("can`t get output file name", 0);
            return 4;
        }

        writeByName(programFileName, code.code, code.programSize);
        free(programFileName);
    }

    free(lines);
    return 0;
}
