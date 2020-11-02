/*
Copyright (c) 2020  MIPT

Module Name:
    Routine.h

Abstract:
    Реализует разные функции

Author:
    JulesIMF

Last Edit:
    22.10.2020 1:42

Edit Notes:

*/

#ifndef _JCPU_ROUTINE
#define _JCPU_ROUTINE

typedef char byte;

/**
 * Возвращает имя выходного файла - *.jbc если asmFileName *.jsm, иначе Program.h
 * \param asmFileName имя файла
 */
char const* getProgramFileName(char const* asmFileName);

/**
 * Убирает ведущие пробелы.
 */
void eraseLeadingSpaces(Line* lines, size_t nLines);

#endif // !_JCPU_ROUTINE
