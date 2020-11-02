/*
Copyright (c) 2020  MIPT

Name:
	Files

Abstract:
	Реализует функции работы с файлами

Author:
	JulesIMF

Last Edit:
	19.09.2020 15:02

Edit Notes:
	1) Подсчет числа слогов в separateStrings
	2) write выводит строфами

	3) replaceSemicolon
*/

///Из-за специфики VS необходимо объявить следующий макрос
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "Line.h"
#include "../Common.h"
#include "../ErrorMessage.h"
//#include "Comparasion.h"

//#define JULESIMF_DEBUG
#define JULESIMF_NO_OUTPUT

int write(char const* fileName, Line* strings, int nStrings, char const* mode)
{
#ifdef    JULESIMF_DEBUG
	assert(fileName);
	assert(strings);
	assert(mode); //Что??
#endif //!JULESIMF_DEBUG

#ifndef   JULESIMF_NO_OUTPUT
	printf("Output file opened\n");
#endif //!JULESIMF_NO_OUTPUT


	//Файл с данными
	int is_stdout = !strcmp(fileName, "?");
	FILE* file = is_stdout ? stdout : fopen(fileName, mode);


	if (!file)
		return 1;

#ifndef   JULESIMF_NO_OUTPUT
	printf("Writing output...\n");
#endif //!JULESIMF_NO_OUTPUT

	//Поехали выводить
	for (int i = 0; i != nStrings; i++)
	{
	#ifdef    JULESIMF_DEBUG
		assert(strings[i].string);
	#endif //!JULESIMF_DEBUG
		if (!(i & 3) && i)
			fprintf(file, "\n");

		fprintf(file, "%s\n", strings[i].string);
	}

#ifndef   JULESIMF_NO_OUTPUT
	printf("Output file closed\n");
#endif //!JULESIMF_NO_OUTPUT

	if (!is_stdout)
		fclose(file);
	return 0;
}


int separateStrings(void* source, int fileSize, Line* destination, int nStrings)
{
#ifdef    JULESIMF_DEBUG
	assert(source);
	assert(destination);
#endif //!JULESIMF_DEBUG

	char* text = (char*)source;
	int   iDestination = 0; //Итератор для destination
	char* currentString = 0;
	int   currentStringLength = 0;
	int nSyllables = 0;

	for (int iText = 0; iText != fileSize + 1; iText++)
	{
		//Если iDestinаtion слишком большой, то JOPA
		if (iDestination >= nStrings)
			return 1;

		//Это если мы в прошлый раз запихнули строку
		if (!currentString)
			currentString = text + iText;

		if (text[iText] == '\n' || !text[iText])
		{
			text[iText] = '\0';
			Line newString =
			{ currentString, currentStringLength };

			destination[iDestination] = newString;
			iDestination++;
			currentString = 0;
			currentStringLength = 0;
			nSyllables = 0;
		}
		else
		{
			//nSyllables += !!isVowel(text[iText]);
			currentStringLength++;
		}
	}

	if (iDestination != nStrings)
		return 1;

	return 0;
}


int getFileSize(FILE* file)
{
#ifdef    JULESIMF_DEBUG
	assert(file);
#endif //!JULESIMF_DEBUG

	fseek(file, 0, SEEK_END);
	int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	return fileSize;
}


void* translateFileIntoRam(char const* fileName, int* pFileSize, int* nStrings)
{
	//Файл с данными
	FILE* file = fopen(fileName, "r");
	if (!file)
	{
		//printf("Failed to open program \"%s\"\n", fileName);
		return 0;
	}
	else
	{
	#ifndef   JULESIMF_NO_OUTPUT
		printf("Input file opened\n");
	#endif //!JULESIMF_NO_OUTPUT
	}

#ifdef    JULESIMF_DEBUG
	assert(file);
	assert(pFileSize);
	assert(nStrings);
#endif //!JULESIMF_DEBUG
	int fileSize = getFileSize(file);
	void* translatedFile = malloc(fileSize + 2); //на всякий случай
	if (!translatedFile)
		return 0;

	int RETURN_fread = fread(translatedFile, 1, fileSize, file);
	((char*)translatedFile)[RETURN_fread] = '\0'; //По новой спецификации функции (чисто поржать)
	fseek(file, 0, SEEK_SET);

	int nNewLines = 0;
	for (int i = 0; i != RETURN_fread; i++)
	{
		if (((char*)translatedFile)[i] == '\n')
			nNewLines++;
	}
	*nStrings = nNewLines + 1;
	*pFileSize = RETURN_fread;

#ifndef   JULESIMF_NO_OUTPUT
	printf("Input file closed\n");
#endif //!JULESIMF_NO_OUTPUT

	fclose(file);
	return translatedFile;
}

void replaceSemicolon(Line* line)
{
	char* rawLine = line->string;

	int size = 0;

	for (; *rawLine; rawLine++)
	{
		if (*rawLine == ';')
		{
			*rawLine = '\0';
			line->size = size;
			return;
		}
		size++;
	}
}

int getLines(char const* fileName, Line** lines, int* nLines)
{
	int fileSize = 0;
	byte* asmFile = (byte*)translateFileIntoRam(fileName, &fileSize, nLines);
	if (asmFile == NULL)
	{
		errorMessage("can`t open the file", 0);
		return 2;
	}

	*lines = (Line*)calloc(*nLines, sizeof(Line));

	if (separateStrings(asmFile, fileSize, *lines, *nLines))
	{
		errorMessage("file translated, but failed to separate lines", 0);
		return 3;
	}

	return 0;
}

int     writeByName(char const* fileName, char* buffer, int size)
{
	FILE* file = fopen(fileName, "wb");

	if (file == NULL)
	{
		errorMessage("can`t open output file for write", 0);
		return 4;
	}

	fwrite(buffer, sizeof(char), size, file);
	fclose(file);
}


void turnToLowercase(Line* line)
{
	for (int i = 0; i != line->size; i++)
		line->string[i] = tolower(line->string[i]);
}