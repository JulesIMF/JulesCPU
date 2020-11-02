#pragma once
#include "../Jules Assembler/Line.h"
/**
 * \brief «агружает файл в RAM и вставл€ет 0 после него, при этом после 0 есть еще один доступный байт.
 *
 * \param fileName загружаемый файл
 * \param fileSize указатель на переменную, в которую будет записан размер файла
 * \param nStrings указатель на переменную, в которую будет записано количество строк
 * \return указатель на область пам€ти, в которую загружен файл, 0 в случае ошибки
 */
void* translateFileIntoRam(char const* fileName, int* pFileSize);


/**
 * \brief ¬ыводит массив строк в файл.
 * \param fileName строка с именем файла ("?" дл€ stdout)
 * \param strings массив строк
 * \param nStrings кличество строк в массиве
 * \param mode режим открыти€ файла
 * \return 0 в случае успешного исполнени€, 1 в случае ошибки открыти€ файла
 */
int write(char const* fileName, Line* strings, int nStrings, char const* mode);


/**
 * \brief ¬озвращает размер файла
 * \param file принимаемый файл
 * \return размер файла в байтах.
 */
int getFileSize(FILE* file);


/**
 * \brief –азбивает source на строки в destination.
 * \param source указатель на транслированный файл
 * \param fileSize размер транслированного файла
 * \param destination указатель на массив со строками
 * \param nStrings количество строк
 * \warning source не перекопируетс€!!
 * \warning массив *stringsLengths перевыдел€етс€!!
 * \return 0 в случае успешного выполнени€, 1 если переданы противоречивые аргументы, 2 если есть строка длиннее 100
 */
int separateStrings(void* source, int fileSize, Line* destination, int nStrings);


/**
 * «амен€ет первый встретившийс€ символ перевода строки, если таковой есть, нулем.
 * \param line строка
 */
void replaceNewLine(char* line);
