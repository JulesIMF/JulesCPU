#include <stdio.h>
#include <Windows.h>


void SetColor(int text, int background)
{
   HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

int main(void)
{
	for(int i = 0; i != 16; i++)
		SetColor(i, 0), printf("Error\n");
	
}
