#include <cstdio>
#include <Windows.h>
#include <conio.h>

void gotoxy(int x, int y)
{
	COORD coordinates = { x, y };
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(outputHandle, coordinates);
}


char buf[10][20];

void bufOut(COORD coord)
{

	for (int i = 0; i != 10; i++)
	{
		gotoxy(coord.X, coord.Y + i);
		for (int j = 0; j != 20; j++)
			_putch(buf[i][j]);


	}

}

COORD GetConsoleCursorPosition(void)
{
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}


int main(void)
{
	for (int i = 0; i != 10; i++) _putch('\n');
	COORD coord = GetConsoleCursorPosition();
	printf("%d %d\n", coord.X, coord.Y);
	_getch();
	coord.Y -= 10;
	bufOut(coord);
	for (int i = 0; i != 10; i++)
		for (int j = 0; j != 20; j++)
			buf[i][j] = 'a' + (i + j)%26;

	system("pause");


	bufOut(coord);
	int n = 0;
	gotoxy(coord.X, coord.Y);
	scanf("%d", &n);
	printf("\n%d", n);
}
