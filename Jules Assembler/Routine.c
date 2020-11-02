/*
Copyright (c) 2020  MIPT

Module Name:
    Parsing.c

Abstract:
    Реализует разные функции

Author:
    JulesIMF

Last Edit:
    22.10.2020 1:42

Edit Notes:

*/

#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Line.h"
#include "Parsing.h"


char const* getProgramFileName(char const* asmFileName)
{
    size_t length = strlen(asmFileName);
    char* programFileName = NULL;
    if (strcmp(".asm", asmFileName + length - 4))
    {
        programFileName = (char*)calloc(12, sizeof(1));
        strcpy(programFileName, "Program.jbc");
        return programFileName;
    }

    programFileName = (char*)calloc(length + 1, sizeof(char));

    if (programFileName == NULL)
        return NULL;

    strcpy(programFileName, asmFileName);
    programFileName[length - 3] = 'j';
    programFileName[length - 2] = 'b';
    programFileName[length - 1] = 'c';
    return programFileName;
}


void eraseLeadingSpaces(Line* lines, size_t nLines)
{
    for (int i = 0; i != nLines; i++)
    {
        if (lines[i].string[0] == DATA_SYMBOL)
            continue;
        char* string = lines[i].string;
        if (i == 65)
        {
            int g = 0;
        }
        for (int j = lines[i].size - 1; j != -1 && isspace(string[j]); j--)
        {
            string[j] = '\0';
            lines[i].size--;
        }

        for (int j = 0; lines[i].size && isspace(string[j]); j++)
        {
            lines[i].string++;
            lines[i].size--;
        }
    }
}