#ifndef INNER_CANVAS_CANVAS_BASE_H
#define INNER_CANVAS_CANVAS_BASE_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "_CORE_Canvas_Type_Define.h"
#include "_CORE_Canvas_Cursor.h"

int _CANVAS_BASIS_ isVisible(p_chr _Chr) { // SPACE (ASCII = 32) is not visible, using to identify transparency
	return ((_Chr - 33) | (126 - _Chr)) >= 0; // _Chr >= 33 && _Chr <= 126;
}
p_chr _CANVAS_BASIS_ topVisibleChr(struct canvas *_Canvas, c_size _XPos, c_size _YPos) {
	if(_XPos < 0 || _YPos < 0 || _Canvas == NULL || _XPos >= _Canvas->width || _YPos >= _Canvas->height) return 0;
	register int _Lay;
	const c_size _Offset = _XPos + _YPos * _Canvas->width;
	for(_Lay = _Canvas->layer * _Canvas->block; _Lay >= 0; _Lay -= _Canvas->block) {
		if(isVisible(_Canvas->point[_Lay + _Offset])) return _Canvas->point[_Lay + _Offset];
	}
	return ' ';
}
l_num _CANVAS_BASIS_ topVisibleLayer(struct canvas *_Canvas, c_size _XPos, c_size _YPos) {
	if(_XPos < 0 || _YPos < 0 || _Canvas == NULL || _XPos >= _Canvas->width || _YPos >= _Canvas->height) return LAYER_NULL;
	register int _Lay;
	const c_size _Offset = _XPos + _YPos * _Canvas->width;
	for(_Lay = _Canvas->layer * _Canvas->block; _Lay >= 0; _Lay -= _Canvas->block) {
		if(isVisible(_Canvas->point[_Lay + _Offset])) return _Lay / _Canvas->block;
	}
	return LAYER_NULL;
}
void _CANVAS_BASIS_ updateCanvas(struct canvas *_Canvas, c_size _LeftPos, c_size _RightPos, c_size _TopPos, c_size _BottomPos) {
	if(_Canvas == NULL) return;
	if(_LeftPos > _RightPos || _TopPos > _BottomPos) return;
	if(_LeftPos < 0 || _LeftPos >= _Canvas->width || _RightPos < 0 || _RightPos >= _Canvas->width) return;
	if(_TopPos < 0 || _TopPos >= _Canvas->height || _BottomPos < 0 || _BottomPos >= _Canvas->height) return;
	register int _IPos, _JPos;
	for(_IPos = _LeftPos; _IPos <= _RightPos; ++_IPos) {
		for(_JPos = _TopPos; _JPos <= _BottomPos; ++_JPos) {
			drawChr(_IPos, _JPos, topVisibleChr(_Canvas, _IPos, _JPos));
		}
	}
	setCursor(0, _Canvas->height + 1);
}
struct canvas _CANVAS_BASIS_ *createCanvasByPureChr(c_size _CanvasWidth, c_size _CanvasHeight, p_chr _BackgroundChr) {
	if(_CanvasWidth <= 0 || _CanvasHeight <= 0) return NULL;
	HANDLE _HStdOut = GetStdHandle(STD_OUTPUT_HANDLE); // TODO for multi-canvas
	SetConsoleScreenBufferSize(_HStdOut, (COORD){_CanvasWidth, _CanvasHeight << 1});
	SetConsoleWindowInfo(_HStdOut, TRUE, &(SMALL_RECT){0, 0, _CanvasWidth - 1, (_CanvasHeight << 1) - 1});
	register int _IPos, _JPos;
	_BackgroundChr = isVisible(_BackgroundChr) ? _BackgroundChr : ' ';
	struct canvas *_Canvas = (struct canvas*)malloc(sizeof(struct canvas));
	_Canvas->width = _CanvasWidth;
	_Canvas->height = _CanvasHeight;
	_Canvas->block = _CanvasWidth * _CanvasHeight;
	_Canvas->layer = 0;
	_Canvas->bgchr = _BackgroundChr;
	_Canvas->point = (p_chr*)malloc(_Canvas->block * sizeof(p_chr));
	_Canvas->area = (struct rect*)malloc(sizeof(struct rect));
	_Canvas->area[0] = (struct rect){0, _CanvasWidth - 1, 0, _CanvasHeight};
	for(_IPos = 0; _IPos < _CanvasWidth; ++_IPos) for(_JPos = 0; _JPos < _CanvasHeight; ++_JPos) {
		_Canvas->point[_IPos + _JPos * _CanvasWidth] = _BackgroundChr;
	}
	updateCanvas(_Canvas, 0, _CanvasWidth - 1, 0, _CanvasHeight - 1);
	return _Canvas;
}
struct graph _CANVAS_BASIS_ createGraph(c_size _GraphWidth, c_size _GraphHeight, p_chr *src) {
	if(_GraphWidth <= 0 || _GraphHeight <= 0 || src == NULL) return GRAPH_NULL;
	struct graph _Graph;
	_Graph.width = _GraphWidth;
	_Graph.height = _GraphHeight;
	_Graph.chr = (p_chr*)malloc(_GraphWidth * _GraphHeight * sizeof(p_chr));
	memcpy(_Graph.chr, src, _GraphWidth * _GraphHeight * sizeof(p_chr));
	return _Graph;
}

#endif