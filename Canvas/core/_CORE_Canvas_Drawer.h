#ifndef INNER_CANVAS_CANVAS_DRAWER_H
#define INNER_CANVAS_CANVAS_DRAWER_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "_CORE_Canvas_Type_Define.h"
#include "_CORE_Canvas_Cursor.h"
#include "_CORE_Canvas_Base.h"

struct graph _CANVAS_FORMAT_DRAWER_ createGraphByModification(struct graph _Graph, p_chr _OriginChr, p_chr _TargetChr) {
	struct graph _NewGraph;
	_NewGraph.width = _Graph.width;
	_NewGraph.height = _Graph.height;
	_NewGraph.chr = (p_chr*)malloc(_Graph.width * _Graph.height * sizeof(p_chr));
	memcpy(_NewGraph.chr, _Graph.chr, _Graph.width * _Graph.height * sizeof(p_chr));
	register int _IPos, _JPos;
	for(_IPos = 0; _IPos < _NewGraph.width; ++_IPos) {
		for(_JPos = 0; _JPos < _NewGraph.height; ++_JPos) {
			if(_NewGraph.chr[_IPos + _JPos * _NewGraph.width] == _OriginChr) {
				_NewGraph.chr[_IPos + _JPos * _NewGraph.width] = _TargetChr;
			}
		}
	}
	return _NewGraph;
}
struct graph _CANVAS_FORMAT_DRAWER_ extractFormatGraphOnLayer(struct canvas *_Canvas, l_num _Layer, c_size _XPos, c_size _YPos, c_size _GraphWidth, c_size _GraphHeight, p_chr _FormatChr, p_chr *_Format) {
	if(_Canvas == NULL || _Layer <= 0 || _Layer > _Canvas->layer || _XPos < 0 || _YPos < 0 || _XPos >= _Canvas->width || _YPos >= _Canvas->height) return GRAPH_NULL;
	if(_GraphWidth <= 0 || _GraphHeight <= 0 || _XPos + _GraphWidth >= _Canvas->width || _YPos + _GraphHeight >= _Canvas->height) return GRAPH_NULL;
	if(!isVisible(_FormatChr) || _Format == NULL) return GRAPH_NULL;
	struct graph _Graph;
	const c_size _PosOffset = _Layer * _Canvas->block + _XPos + _YPos * _Canvas->width;
	register int _IPos, _JPos;
	_Graph.width = _GraphWidth;
	_Graph.height = _GraphHeight;
	_Graph.chr = (p_chr*)malloc(_GraphWidth * _GraphHeight * sizeof(p_chr));
	memcpy(_Graph.chr, _Format, _GraphWidth * _GraphHeight * sizeof(p_chr));
	for(_IPos = 0; _IPos < _GraphWidth; ++_IPos) {
		for(_JPos = 0; _JPos < _GraphHeight; ++_JPos) {
			if(_Graph.chr[_IPos + _JPos * _Graph.width] == _FormatChr) {
				_Graph.chr[_IPos + _JPos * _Graph.width] = _Canvas->point[_PosOffset + _IPos + _JPos * _Canvas->width];
			}
		}
	}
	return _Graph;
}
void _CANVAS_FORMAT_DRAWER_ drawFormatGraphOnLayer(struct canvas *_Canvas, l_num _Layer, struct graph _Graph, c_size _XPos, c_size _YPos, p_chr _FormatChr, p_chr *_Format) {
	if(_Canvas == NULL || _Graph.chr == NULL || _Graph.width <= 0 || _Graph.height <= 0) return;
	if(_Layer <= 0 || _XPos >= _Canvas->width || _YPos >= _Canvas->height) return;
	if(!isVisible(_FormatChr) || _Format == NULL) return;
	const c_size _PosOffset = _Layer * _Canvas->block + _XPos + _YPos * _Canvas->width;
	register int _IPos, _JPos;
	for(_IPos = 0; _IPos < _Graph.width; ++_IPos) {
		if(_XPos + _IPos >= _Canvas->width || _XPos + _IPos < 0) continue;
		for(_JPos = 0; _JPos < _Graph.height; ++_JPos) {
			if(_YPos + _JPos >= _Canvas->height || _YPos + _JPos < 0) continue;
			if(_Format[_IPos + _JPos * _Graph.width] == _FormatChr) {
				_Canvas->point[_PosOffset + _IPos + _JPos * _Canvas->width] = _Graph.chr[_IPos + _JPos * _Graph.width];
			}
		}
	}
	const c_size _Left = max(_XPos, 0);
	const c_size _Top = max(_YPos, 0);
	const c_size _Right = min(_XPos + _Graph.width - 1, _Canvas->width - 1);
	const c_size _Bottom = min(_YPos + _Graph.height - 1, _Canvas->height - 1);
	_Canvas->area[_Layer].left = min(_Left, _Canvas->area[_Layer].left);
	_Canvas->area[_Layer].top = min(_Top, _Canvas->area[_Layer].top);
	_Canvas->area[_Layer].right = max(_Right, _Canvas->area[_Layer].right);
	_Canvas->area[_Layer].bottom = max(_Bottom, _Canvas->area[_Layer].bottom);
	updateCanvas(_Canvas, _Left, _Right, _Top, _Bottom);
}
void _CANVAS_FORMAT_DRAWER_ eraseFormatGraphOnLayer(struct canvas *_Canvas, l_num _Layer, c_size _XPos, c_size _YPos, c_size _RectWidth, c_size _RectHeight, p_chr _FormatChr, p_chr *_Format) {
	if(_Canvas == NULL || _Layer <= 0 || _Layer > _Canvas->layer || _XPos < 0 || _YPos < 0 || _RectWidth <= 0 || _RectHeight <= 0) return;
	if(_XPos + _RectWidth >= _Canvas->width || _YPos + _RectHeight >= _Canvas->height) return;
	if(!isVisible(_FormatChr) || _Format == NULL) return;
	register int _IPos, _JPos;
	const c_size _LayerOffset = _Layer * _Canvas->block;
	const c_size _PosOffset = _LayerOffset + _XPos + _YPos * _Canvas->width;
	for(_IPos = 0; _IPos < _RectWidth; ++_IPos) {
		for(_JPos = 0; _JPos < _RectHeight; ++_JPos) {
			if(_Format[_IPos + _JPos * _RectWidth] == _FormatChr) {
				_Canvas->point[_PosOffset + _IPos + _JPos * _Canvas->width] = ' ';
			}
		}
	}
	int _Left, _Right, _Top, _Bottom;
	_Left = _Right = _Top = _Bottom = 0;
	for(_IPos = _Canvas->area[_Layer].left; _IPos <= _Canvas->area[_Layer].right; ++_IPos) {
		for(_JPos = _Canvas->area[_Layer].top; _JPos <= _Canvas->area[_Layer].bottom; ++_JPos) {
			if(_Canvas->point[_LayerOffset + _IPos + _JPos * _Canvas->width] != ' ') {
				if(!_Left) {
					_Left = 1;
					_Canvas->area[_Layer].left = _IPos;
				} else {
					_Canvas->area[_Layer].right = _IPos;
				}
				_Canvas->area[_Layer].top = min(_JPos, _Canvas->area[_Layer].top);
				_Canvas->area[_Layer].bottom = max(_JPos, _Canvas->area[_Layer].bottom);
			}
		}
	}
	updateCanvas(_Canvas, _XPos, _XPos + _RectWidth - 1, _YPos, _YPos + _RectHeight - 1);
}


void _CANVAS_RECT_DRAWER_ drawRectGraphOnTop(struct canvas *_Canvas, struct graph _Graph, c_size _XPos, c_size _YPos) {
	if(_Canvas == NULL || _Canvas->layer == CANVAS_LAYER_MAX || _Graph.chr == NULL || _Graph.width <= 0 || _Graph.height <= 0) return;
	if(_XPos >= _Canvas->width || _YPos >= _Canvas->height) return;
	register int _IPos, _JPos;
	_Canvas->layer++;
	const c_size _LayerOffset = _Canvas->layer * _Canvas->block;
	const c_size _PosOffset = _LayerOffset + _XPos + _YPos * _Canvas->width;
	_Canvas->point = (p_chr*)realloc(_Canvas->point, (_Canvas->layer + 1) * _Canvas->block * sizeof(p_chr));
	_Canvas->area = (struct rect*)realloc(_Canvas->area, (_Canvas->layer + 1) * sizeof(struct rect));
	for(_IPos = 0; _IPos < _Canvas->width; ++_IPos) for(_JPos = 0; _JPos < _Canvas->height; ++_JPos) _Canvas->point[_LayerOffset + _IPos + _JPos * _Canvas->width] = ' ';
	for(_IPos = 0; _IPos < _Graph.width; ++_IPos) {
		if(_XPos + _IPos >= _Canvas->width || _XPos + _IPos < 0) continue;
		for(_JPos = 0; _JPos < _Graph.height; ++_JPos) {
			if(_YPos + _JPos >= _Canvas->height || _YPos + _JPos < 0) continue;
			_Canvas->point[_PosOffset + _IPos + _JPos * _Canvas->width] = _Graph.chr[_IPos + _JPos * _Graph.width];
		}
	}
	_Canvas->area[_Canvas->layer] = (struct rect){
		max(_XPos, 0),
		min(_XPos + _Graph.width - 1, _Canvas->width - 1),
		max(_YPos, 0),
		min(_YPos + _Graph.height - 1, _Canvas->height - 1)
	};
	updateCanvas(_Canvas, _Canvas->area[_Canvas->layer].left, _Canvas->area[_Canvas->layer].right, _Canvas->area[_Canvas->layer].top, _Canvas->area[_Canvas->layer].bottom);
}
struct graph _CANVAS_RECT_DRAWER_ extractRectGraphOnLayer(struct canvas *_Canvas, l_num _Layer, c_size _XPos, c_size _YPos, c_size _RectWidth, c_size _RectHeight) {
	if(_Canvas == NULL || _Layer <= 0 || _Layer > _Canvas->layer || _XPos < 0 || _YPos < 0 || _XPos >= _Canvas->width || _YPos >= _Canvas->height) return GRAPH_NULL;
	if(_RectWidth <= 0 || _RectHeight <= 0 || _XPos + _RectWidth > _Canvas->width || _YPos + _RectHeight > _Canvas->height) return GRAPH_NULL;
	struct graph _Graph;
	const c_size _LayerOffset = _Layer * _Canvas->block;
	const c_size _PosOffset = _LayerOffset + _XPos + _YPos * _Canvas->width;
	register int _IPos, _JPos;
	_Graph.width = _RectWidth;
	_Graph.height = _RectHeight;
	_Graph.chr = (p_chr*)malloc(_RectWidth * _RectHeight * sizeof(p_chr));
	for(_IPos = 0; _IPos < _RectWidth; ++_IPos) {
		for(_JPos = 0; _JPos < _RectHeight; ++_JPos) {
			_Graph.chr[_IPos + _JPos * _Graph.width] = _Canvas->point[_PosOffset + _IPos + _JPos * _Canvas->width];
		}
	}
	return _Graph;
}
void _CANVAS_RECT_DRAWER_ drawRectGraphOnLayer(struct canvas *_Canvas, l_num _Layer, struct graph _Graph, c_size _XPos, c_size _YPos, int _Mode) {
	if(_Canvas == NULL || _Graph.chr == NULL || _Graph.width <= 0 || _Graph.height <= 0) return;
	if(_Layer <= 0 || _XPos >= _Canvas->width || _YPos >= _Canvas->height) return;
	register int _IPos, _JPos;
	const c_size _LayerOffset = _Layer * _Canvas->block;
	const c_size _PosOffset = _LayerOffset + _XPos + _YPos * _Canvas->width;
	for(_IPos = 0; _IPos < _Graph.width; ++_IPos) {
		if(_XPos + _IPos >= _Canvas->width || _XPos + _IPos < 0) continue;
		for(_JPos = 0; _JPos < _Graph.height; ++_JPos) {
			if(_YPos + _JPos >= _Canvas->height || _YPos + _JPos < 0) continue;
			if(_Mode == CANVAS_LAYER_DRAW_SKIP && isVisible(_Canvas->point[_PosOffset + _IPos + _JPos * _Canvas->width])) continue;
			_Canvas->point[_PosOffset + _IPos + _JPos * _Canvas->width] = _Graph.chr[_IPos + _JPos * _Graph.width];
		}
	}
	const c_size _Left = max(_XPos, 0);
	const c_size _Top = max(_YPos, 0);
	const c_size _Right = min(_XPos + _Graph.width - 1, _Canvas->width - 1);
	const c_size _Bottom = min(_YPos + _Graph.height - 1, _Canvas->height - 1);
	_Canvas->area[_Layer].left = min(_Left, _Canvas->area[_Layer].left);
	_Canvas->area[_Layer].top = min(_Top, _Canvas->area[_Layer].top);
	_Canvas->area[_Layer].right = max(_Right, _Canvas->area[_Layer].right);
	_Canvas->area[_Layer].bottom = max(_Bottom, _Canvas->area[_Layer].bottom);
	updateCanvas(_Canvas, _Left, _Right, _Top, _Bottom);
}
void _CANVAS_RECT_DRAWER_ eraseRectGraphOnLayer(struct canvas *_Canvas, l_num _Layer, c_size _XPos, c_size _YPos, c_size _RectWidth, c_size _RectHeight) {
	if(_Canvas == NULL || _Layer <= 0 || _Layer > _Canvas->layer || _XPos < 0 || _YPos < 0 || _RectWidth <= 0 || _RectHeight <= 0) return;
	if(_XPos + _RectWidth >= _Canvas->width || _YPos + _RectHeight >= _Canvas->height) return;
	register int _IPos, _JPos;
	const c_size _LayerOffset = _Layer * _Canvas->block;
	const c_size _PosOffset = _LayerOffset + _XPos + _YPos * _Canvas->width;
	for(_IPos = 0; _IPos < _RectWidth; ++_IPos) {
		for(_JPos = 0; _JPos < _RectHeight; ++_JPos) {
			_Canvas->point[_PosOffset + _IPos + _JPos * _Canvas->width] = ' ';
		}
	}
	int _Left, _Right, _Top, _Bottom;
	_Left = _Right = _Top = _Bottom = 0;
	for(_IPos = _Canvas->area[_Layer].left; _IPos <= _Canvas->area[_Layer].right; ++_IPos) {
		for(_JPos = _Canvas->area[_Layer].top; _JPos <= _Canvas->area[_Layer].bottom; ++_JPos) {
			if(_Canvas->point[_LayerOffset + _IPos + _JPos * _Canvas->width] != ' ') {
				if(!_Left) {
					_Left = 1;
					_Canvas->area[_Layer].left = _IPos;
				} else {
					_Canvas->area[_Layer].right = _IPos;
				}
				_Canvas->area[_Layer].top = min(_JPos, _Canvas->area[_Layer].top);
				_Canvas->area[_Layer].bottom = max(_JPos, _Canvas->area[_Layer].bottom);
			}
		}
	}
	updateCanvas(_Canvas, _XPos, _XPos + _RectWidth - 1, _YPos, _YPos + _RectHeight - 1);
}

#endif