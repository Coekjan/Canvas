#ifndef INNER_CANVAS_CANVAS_IOSTREAM_H
#define INNER_CANVAS_CANVAS_IOSTREAM_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "_CORE_Canvas_Type_Define.h"
#include "_CORE_Canvas_Cursor.h"
#include "_CORE_Canvas_Base.h"
#include "_CORE_Canvas_Drawer.h"
#include "_CORE_Canvas_Layer.h"

struct graph _CANVAS_IOSTREAM_ importGraphFromCANVGFile(const char *_InStreamPath) {
	FILE *_InStream = fopen(_InStreamPath, "r");
	if(_InStream == NULL) return GRAPH_NULL;
	char _FormatHead[CANVAS_CANVG_FORMAT_HEAD_LENGTH];
	c_size _Width, _Height;
	fscanf(_InStream, CANVAS_CANVG_FORMAT_IN, _FormatHead, &_Width, &_Height);
	if(strcmp(_FormatHead, CANVAS_CANVG_FORMAT_HEAD) != 0) return GRAPH_NULL;
	struct graph _Graph;
	_Graph.width = _Width;
	_Graph.height = _Height;
	_Graph.chr = (p_chr*)malloc(_Width * _Height * sizeof(p_chr));
	register int _IPos, _JPos;
	fgetc(_InStream);
	for(_JPos = 0; _JPos < _Graph.height; ++_JPos) {
		for(_IPos = 0; _IPos < _Graph.width; ++_IPos) {
			_Graph.chr[_IPos + _JPos * _Graph.width] = fgetc(_InStream);
		}
		fgetc(_InStream);
	}
	fclose(_InStream);
	return _Graph;
}
void _CANVAS_IOSTREAM_ exportGraphToCANVGFile(const char *_OutStreamPath, struct graph _Graph) {
	if(_Graph.chr == GRAPH_CHR_NULL) return;
	if(memcmp(_OutStreamPath, CANVAS_HEAD_PATH "/", strlen(CANVAS_HEAD_PATH "/") * sizeof(char)) == 0) return;
	FILE *_OutStream = fopen(_OutStreamPath, "w");
	if(_OutStreamPath == NULL) return;
	fprintf(_OutStream, CANVAS_CANVG_FORMAT_OUT"\n", CANVAS_CANVG_FORMAT_HEAD, _Graph.width, _Graph.height);
	register int _IPos, _JPos;
	for(_JPos = 0; _JPos < _Graph.height; ++_JPos) {
		for(_IPos = 0; _IPos < _Graph.width; ++_IPos) {
			fputc(_Graph.chr[_IPos + _JPos * _Graph.width], _OutStream);
		}
		fputc('\n', _OutStream);
	}
	fclose(_OutStream);
}
void _CANVAS_IOSTREAM_ exportLayerToCANVGFILE(const char *_OutStreamPath, struct canvas *_Canvas, l_num _Layer, int mode) {
	struct graph _Graph;
	if(mode == CANVAS_LAYER_EXPORT_WHOLE) {
		_Graph = extractRectGraphOnLayer(_Canvas, _Layer, 0, 0, _Canvas->width, _Canvas->height);
	} else {
		_Graph = extractRectGraphOnLayer(_Canvas, _Layer, _Canvas->area[_Layer].left, _Canvas->area[_Layer].top, _Canvas->area[_Layer].right - _Canvas->area[_Layer].left + 1, _Canvas->area[_Layer].bottom - _Canvas->area[_Layer].top + 1);
	}
	exportGraphToCANVGFile(_OutStreamPath, _Graph);
	free(_Graph.chr);
}


#endif