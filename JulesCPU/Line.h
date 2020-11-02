#ifndef JULESIMF_LINE_INCLUDED
#define JULESIMF_LINE_INCLUDED
/**
 * Структура, хранящая указатель на строку и ее предподсчитанный размер.
 */
typedef struct string_view_
{
	char* string;
	int nSyllables;
	int size;
} Line;
#endif // !JULESIMF_LINE_INCLUDED
