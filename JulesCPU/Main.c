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
#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include "Core.h"
#include "../Common.h"
#include "Files.h"
#include "Execution.h"
#include "../ErrorMessage.h"

static size_t   RAM_CAPACITY = 10000;
static size_t   VIDEO_X = 50,
                VIDEO_Y = 25;



int main(int argc, char const** argv)
{
    setColor(White);

    if (argc == 1)
    {
        errorMessage("argc == 1", 0);
        return 1;

        //DEBUG
        setColor(White);
        argv[1] = "../pgmcmn/newCircle.jbc";
    }
    Core* core = newCore(RAM_CAPACITY, VIDEO_X, VIDEO_Y);
    int fileSize = 0;
    byte* file = (byte*)translateFileIntoRam(argv[1], &fileSize);
    if (file == NULL)
    {
        errorMessage("can`t open file", 0);
        return 2;
    }

    if (runProgram(core, file, fileSize))
        return 3;

    int opcode = 0;

    while ((opcode = executeNextCommand(core)) != -1)
    {
        
    }
        

    deleteCore(core);
    free(file);
    setColor(LightGray);
    //_getch();
    return 0;
}
