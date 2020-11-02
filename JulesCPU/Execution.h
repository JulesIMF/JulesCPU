/*
Copyright (c) 2020  MIPT

Module Name:
    Execution.h

Abstract:
    Описывает интерфейс обработки программы

Author:
    JulesIMF

Last Edit:
    16.10.2020 11:29

Edit Notes:

*/

#ifndef _JCPU_EXECUTION
#define _JCPU_EXECUTION
#include "Core.h"
#include "IRQ.h"



/**
 * Проверяет, возможно ли получить аргумент соответствующего размера.
 *
 * \param core
 * \param size
 * \return 0 если возможно (не конец ленты), 1 иначе
 */
int isTapeEnd(Core* core, size_t size);


//Если isValidParameters == 0, остальные поля могут быть какими угодно
typedef struct
{
    int isValidParameters;
    long long first;
    long long second;
} Parameters;


Program getProgram(byte* byteCode, size_t byteCodeSize, Core* core);

typedef struct
{
    int isPtr_1;
    int isPtr_2;
    int AST_1;
    int AST_2;
} SecondByte;

SecondByte getSecondByte(Core* core);
int popBySecondByte(Core* core);


Parameters getParameters(Core* core, int nParameters);


/**
 * Запускает программу. Может выводить сообщения об ошибках
 *
 * \param core
 * \param translatedFile
 * \param fileSize
 * \return 0 если программа запущена без ошибок, 1 иначе
 */
int runProgram(Core* core, byte* translatedFile, int fileSize);

/**
 * .
 *
 * \param core
 * \return 0 если программа больше не может исполняться, 1 иначе
 */
int executeNextCommand(Core* core);

#endif // !_JCPU_EXECUTION
