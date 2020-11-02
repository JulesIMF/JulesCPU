/*
Copyright (c) 2020  MIPT

Name:
	io.c

Abstract:
	Реализует функции ввода и вывода

Author:
	JulesIMF

Last Edit:
	31.10.2020

Edit Notes:

*/

#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include "Core.h"

static isControlSymbol(char c)
{
	return
		c == '\0' ||
		c == '\n' ||
		c == '\r' ||
		c == '\t' ||
		c == '\v' ||
		c == '\b' ||
		c == '\v' ||
		c == '\f';
}


void GotoXY(int x, int y)
{
	COORD coordinates = { x, y };
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(outputHandle, coordinates);
}

void cpu_normalize(Core* core)
{
	if (core->videoX == core->currentX)
		core->currentY++, core->currentX = 0;
	if (core->videoY == core->currentY)
		core->currentY = 0, GotoXY(core->coord.X, core->coord.Y);
}


void cpu_PrnBuf(Core* core)
{
	int shift = 0;
	long long* ram = core->ram;
	for (int i = 0; i != core->videoY; i++)
	{
		GotoXY(core->coord.X, core->coord.Y + i);
		for (int j = 0; j != core->videoX; j++)
			if (!isControlSymbol(ram[shift]))
				_putch(ram[shift++]);
			else
				_putch(' '), shift++;
	}
	GotoXY(core->coord.X + core->currentX, core->coord.Y + core->currentY);

}

int cpu_getConsoleCursorPosition(Core* core)
{
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		COORD coord = cbsi.dwCursorPosition;
		coord.Y -= core->videoY;
		if (coord.Y < 0)
			coord.Y = 0;
		core->coord = coord;
		return 1;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		return 0;
	}
}

void cpu_cls(Core* core)
{
	int maxShift = core->videoX * core->videoY;
	long long* ram = core->ram;
	for (int i = 0; i != maxShift; i++)
		ram[i] = ' ';
	core->currentX = core->currentY = 0;
	cpu_PrnBuf(core);
}

void cpu_printf(Core* core, char const* src)
{
	int videoX = core->videoX,
		videoY = core->videoY,
		currentX = core->currentX,
		currentY = core->currentY;

	int shift = videoX * currentY + currentX;

	long long* ram = core->ram;

	while (*src != '\0')
	{
		if (isControlSymbol(*src))
			ram[shift++] = ' ';
		else
			ram[shift++] = *src;
		src++;
		currentX++;
		if (videoX == currentX)
			currentY++, currentX = 0;
		if (videoY == currentY)
			currentY = 0, shift = 0;
	}
	core->currentX = currentX;
	core->currentY = currentY;
	cpu_PrnBuf(core);
}
