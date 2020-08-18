#ifndef INNER_CANVAS_CANVAS_CURSOR_H
#define INNER_CANVAS_CANVAS_CURSOR_H

#include <windows.h>
#include "_CORE_Canvas_Type_Define.h"

void _CANVAS_CURSOR_DRAWER_ setCursor(c_size _X, c_size _Y) {
	HANDLE _HStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD _Coord = {_X, _Y};
	SetConsoleCursorPosition(_HStdOut, _Coord);
}
void _CANVAS_CURSOR_DRAWER_ moveCursor(c_size _DeltaX, c_size _DeltaY) {
	HANDLE _HStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO _BufferInfo;
	GetConsoleScreenBufferInfo(_HStdOut, &_BufferInfo);
	COORD _Coord = {_BufferInfo.dwCursorPosition.X + _DeltaX, _BufferInfo.dwCursorPosition.Y + _DeltaY};
	SetConsoleCursorPosition(_HStdOut, _Coord);
}
void _CANVAS_CURSOR_DRAWER_ drawChr(c_size _X, c_size _Y, p_chr _Chr) {
	HANDLE _HStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD _TempN;
	FillConsoleOutputCharacterW(_HStdOut, (WCHAR)_Chr, 1, (COORD){_X, _Y}, &_TempN);
}
p_chr _CANVAS_CURSOR_DRAWER_ absrChr(c_size _X, c_size _Y) {
	HANDLE _HStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WCHAR _Chr;
	DWORD _TempN;
	ReadConsoleOutputCharacterW(_HStdOut, &_Chr, 1, (COORD){_X, _Y}, &_TempN);
	return (p_chr)_Chr;
}


#endif