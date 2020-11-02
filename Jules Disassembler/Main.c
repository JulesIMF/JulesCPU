/*
Copyright (c) 2020  MIPT

Module Name:
    Main.c

Abstract:
    Основной файл

Author:
    JulesIMF

Last Edit:
    19.10.2020 1:51

Edit Notes:

*/

#include <stdio.h>
#include <stdlib.h>
#include "../Common.h"
#include "Includes.h"
#include "Files.h"
#include "../ErrorMessage.h"

int main(int argc, char const** argv)
{
    if (argc == 1)
    {
        errorMessage("no *.jbc file specified", 0);
        return 0;

        //DEBUG
        argv[1] = "../pgmcmn/Demo/newCircle.jbc";
    }
    int fileSize = 0;
    byte* file = (byte*)translateFileIntoRam(argv[1], &fileSize);
    if (file == NULL)
    {
        errorMessage("can't open file", 0);
        return 0;
    }

    disassembly(file, fileSize, argv[1]);
}
