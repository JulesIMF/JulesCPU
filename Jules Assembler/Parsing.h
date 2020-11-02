/*
Copyright (c) 2020  MIPT

Module Name:
    Parsing.h

Abstract:


Author:
    JulesIMF

Last Edit:
    22.10.2020

Edit Notes:

*/

#ifndef _JCPU_PARSING
#define _JCPU_PARSING



#define LABEL_SYMBOL     ':'
#define STACKSET_SYMBOL  '@'
#define INTSET_SYMBOL    '#'
#define DATA_SYMBOL      '~'

typedef struct
{
    char const* label;
    int address;
} Label;

typedef struct
{
    size_t nLabels;
    Label* labels;
    long long stackSize;
    long long programSize;
    int IRQ[8];
} ProgramParameters;

typedef struct
{
    size_t length;

    char code[20];

    //
    //Здесь непонятно, что это за команда, какие у нее аргументы,
    //почему такой length... Мне вообще это не интересно, мне интересно какая
    //длина у команды + аргументов, а потом я все это вытащу из массива code.
    //20 байт должно хватить
    //
} Instruction;


/**
 * Берет строки и выцепляет размер программы и массив лейблов.
 * Если программа некорректна, поле programSize принимает значение -1.
 */
ProgramParameters   getProgramParameters(Line* lines, size_t nLines);
Instruction         getInstruction(Line line, size_t lineN, Label* labels, size_t nLabels);

unsigned long long   isStackSet(Line line);
char const* isLabel(Line line);
long long            isIntSet(Line line);
#endif // !_JCPU_PARSING
