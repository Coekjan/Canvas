#ifndef INNER_CANVAS_CANVAS_LAYER_H
#define INNER_CANVAS_CANVAS_LAYER_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "_CORE_Canvas_Type_Define.h"
#include "_CORE_Canvas_Cursor.h"
#include "_CORE_Canvas_Base.h"
#include "_CORE_Canvas_Drawer.h"

void _CANVAS_LAYER_ removeLayer(struct canvas *_Canvas, l_num _Layer) {
	if(_Canvas == NULL) return;
	if(_Layer <= 0) return;
	memmove(_Canvas->point + _Layer * _Canvas->block, _Canvas->point + (_Layer + 1) * _Canvas->block, (_Canvas->layer - _Layer) * _Canvas->block * sizeof(p_chr));
	_Canvas->point = (p_chr*)realloc(_Canvas->point, _Canvas->layer * _Canvas->block * sizeof(p_chr));
	updateCanvas(_Canvas, _Canvas->area[_Layer].left, _Canvas->area[_Layer].right, _Canvas->area[_Layer].top, _Canvas->area[_Layer].bottom);
	memmove(_Canvas->area + _Layer, _Canvas->area + _Layer + 1, (_Canvas->layer - _Layer) * sizeof(struct rect));
	_Canvas->area = (struct rect*)realloc(_Canvas->area, _Canvas->layer * sizeof(struct rect));
	_Canvas->layer--;
}
void _CANVAS_LAYER_ moveLayer(struct canvas *_Canvas, l_num _Layer, c_size _DeltaX, c_size _DeltaY) {
	if(_Canvas == NULL) return;
	if(_Layer <= 0 || _Layer > _Canvas->layer) return;
	if(!_DeltaX && !_DeltaY) return;
	if(_Canvas->area[_Layer].left + _DeltaX < 0 || _Canvas->area[_Layer].right + _DeltaX < 0 || _Canvas->area[_Layer].left + _DeltaX >= _Canvas->width || _Canvas->area[_Layer].right + _DeltaX >= _Canvas->width) _DeltaX = 0;
	if(_Canvas->area[_Layer].top + _DeltaY < 0 || _Canvas->area[_Layer].bottom + _DeltaY < 0 || _Canvas->area[_Layer].top + _DeltaY >= _Canvas->height || _Canvas->area[_Layer].bottom + _DeltaY >= _Canvas->height) _DeltaY = 0;
	register int _IPos, _JPos;
	const c_size _PosOffset = _Layer * _Canvas->block;
	if(_DeltaX < 0) {
		for(_IPos = _PosOffset - _DeltaX; _IPos < _PosOffset + _Canvas->width; ++_IPos) {
			for(_JPos = 0; _JPos < _Canvas->height; ++_JPos) {
				_Canvas->point[_IPos + _DeltaX + _JPos * _Canvas->width] = _Canvas->point[_IPos + _JPos * _Canvas->width];
			}
		}
		for(_IPos = _PosOffset + _Canvas->width + _DeltaX; _IPos < _PosOffset + _Canvas->width; ++_IPos) {
			for(_JPos = 0; _JPos < _Canvas->height; ++_JPos) {
				_Canvas->point[_IPos + _JPos * _Canvas->width] = ' ';
			}
		}
	} else if(_DeltaX > 0) {
		for(_IPos = _PosOffset + _Canvas->width - _DeltaX - 1; _IPos >= _PosOffset; --_IPos) {
			for(_JPos = 0; _JPos < _Canvas->height; ++_JPos) {
				_Canvas->point[_IPos + _DeltaX + _JPos * _Canvas->width] = _Canvas->point[_IPos + _JPos * _Canvas->width];
			}
		}
		for(_IPos = _PosOffset + _DeltaX - 1; _IPos >= _PosOffset; --_IPos) {
			for(_JPos = 0; _JPos < _Canvas->height; ++_JPos) {
				_Canvas->point[_IPos + _JPos * _Canvas->width] = ' ';
			}
		}
	}
	if(_DeltaY < 0) {
		for(_JPos = -_DeltaY; _JPos < _Canvas->height; ++_JPos) {
			for(_IPos = _PosOffset; _IPos < _PosOffset + _Canvas->width; ++_IPos) {
				_Canvas->point[_IPos + (_JPos + _DeltaY) * _Canvas->width] = _Canvas->point[_IPos + _JPos * _Canvas->width];
			}
		}
		for(_JPos = _Canvas->height + _DeltaY; _JPos < _Canvas->height; ++_JPos) {
			for(_IPos = _PosOffset; _IPos < _PosOffset + _Canvas->width; ++_IPos) {
				_Canvas->point[_IPos + _JPos * _Canvas->width] = ' ';
			}
		}
	} else if(_DeltaY > 0) {
		for(_JPos = _Canvas->height - _DeltaY - 1; _JPos >= 0; --_JPos) {
			for(_IPos = _PosOffset; _IPos < _PosOffset + _Canvas->width; ++_IPos) {
				_Canvas->point[_IPos + (_JPos + _DeltaY) * _Canvas->width] = _Canvas->point[_IPos + _JPos * _Canvas->width];
			}
		}
		for(_JPos = _DeltaY - 1; _JPos >= 0; --_JPos) {
			for(_IPos = _PosOffset; _IPos < _PosOffset + _Canvas->width; ++_IPos) {
				_Canvas->point[_IPos + _JPos * _Canvas->width] = ' ';
			}
		}
	}
	updateCanvas(_Canvas, _Canvas->area[_Layer].left, _Canvas->area[_Layer].right, _Canvas->area[_Layer].top, _Canvas->area[_Layer].bottom);
	_Canvas->area[_Layer].left = _Canvas->area[_Layer].left + _DeltaX;
	_Canvas->area[_Layer].right = _Canvas->area[_Layer].right + _DeltaX;
	_Canvas->area[_Layer].top = _Canvas->area[_Layer].top + _DeltaY;
	_Canvas->area[_Layer].bottom = _Canvas->area[_Layer].bottom + _DeltaY;
	updateCanvas(_Canvas, _Canvas->area[_Layer].left, _Canvas->area[_Layer].right, _Canvas->area[_Layer].top, _Canvas->area[_Layer].bottom);
}
void _CANVAS_LAYER_ changeLayerDisplayMode(struct canvas *_Canvas, l_num _Layer) { // hide or display
	if(_Canvas == NULL) return;
	if(_Layer <= 0) return;
	register int _IPos, _JPos;
	const c_size _PosOffset = _Layer * _Canvas->block;
	for(_IPos = 0; _IPos < _Canvas->width; ++_IPos) {
		for(_JPos = 0; _JPos < _Canvas->height; ++_JPos) {
			_Canvas->point[_PosOffset + _IPos + _JPos * _Canvas->width] = ~(_Canvas->point[_PosOffset + _IPos + _JPos * _Canvas->width]);
		}
	}
	updateCanvas(_Canvas, _Canvas->area[_Layer].left, _Canvas->area[_Layer].right, _Canvas->area[_Layer].top, _Canvas->area[_Layer].bottom);
}
void _CANVAS_LAYER_ combineLayer(struct canvas *_Canvas, l_num _DestLayer, l_num _SrcLayer, int _Mode) {
	if(_Canvas == NULL) return;
	if(_DestLayer <= 0 || _SrcLayer <= 0 || _DestLayer == _SrcLayer || _DestLayer > _Canvas->layer || _SrcLayer > _Canvas->layer) return;
	const c_size _DestOffset = _DestLayer * _Canvas->block;
	const c_size _SrcOffset = _SrcLayer * _Canvas->block;
	register int _IPos, _JPos;
	for(_IPos = 0; _IPos < _Canvas->width; ++_IPos) {
		for(_JPos = 0; _JPos < _Canvas->height; ++_JPos) {
			if(_Mode == CANVAS_LAYER_DRAW_SKIP && isVisible(_Canvas->point[_DestOffset + _IPos + _JPos * _Canvas->width])) continue;
			_Canvas->point[_DestOffset + _IPos + _JPos * _Canvas->width] = _Canvas->point[_SrcOffset + _IPos + _JPos * _Canvas->width];
		}
	}
	_Canvas->area[_DestLayer].left = min(_Canvas->area[_DestLayer].left, _Canvas->area[_SrcLayer].left);
	_Canvas->area[_DestLayer].top = min(_Canvas->area[_DestLayer].top, _Canvas->area[_SrcLayer].top);
	_Canvas->area[_DestLayer].right = max(_Canvas->area[_DestLayer].right, _Canvas->area[_SrcLayer].right);
	_Canvas->area[_DestLayer].bottom = max(_Canvas->area[_DestLayer].bottom, _Canvas->area[_SrcLayer].bottom);
	memmove(_Canvas->point + _SrcLayer * _Canvas->block, _Canvas->point + (_SrcLayer + 1) * _Canvas->block, (_Canvas->layer - _SrcLayer) * _Canvas->block * sizeof(p_chr));
	_Canvas->point = (p_chr*)realloc(_Canvas->point, _Canvas->layer * _Canvas->block * sizeof(p_chr));
	memmove(_Canvas->area + _SrcLayer, _Canvas->area + _SrcLayer + 1, (_Canvas->layer - _SrcLayer) * sizeof(struct rect));
	_Canvas->area = (struct rect*)realloc(_Canvas->area, _Canvas->layer * sizeof(struct rect));
	_Canvas->layer--;
}
void _CANVAS_LAYER_ swapLayer(struct canvas *_Canvas, l_num _Layer1, l_num _Layer2) {
	if(_Canvas == NULL || _Layer1 <= 0 || _Layer2 <= 0 || _Layer1 > _Canvas->layer || _Layer2 > _Canvas->layer) return;
	p_chr *_TempLayer = (p_chr*)malloc(_Canvas->block * sizeof(p_chr));
	memmove(_TempLayer, &(_Canvas->point[_Layer1 * _Canvas->block]), _Canvas->block * sizeof(p_chr));
	memmove(&(_Canvas->point[_Layer1 * _Canvas->block]), &(_Canvas->point[_Layer2 * _Canvas->block]), _Canvas->block * sizeof(p_chr));
	memmove(&(_Canvas->point[_Layer2 * _Canvas->block]), _TempLayer, _Canvas->block * sizeof(p_chr));
	free(_TempLayer);
	updateCanvas(_Canvas, _Canvas->area[_Layer1].left, _Canvas->area[_Layer1].right, _Canvas->area[_Layer1].top, _Canvas->area[_Layer1].bottom);
	updateCanvas(_Canvas, _Canvas->area[_Layer2].left, _Canvas->area[_Layer2].right, _Canvas->area[_Layer2].top, _Canvas->area[_Layer2].bottom);
}
void _CANVAS_LAYER_ flowLayer(struct canvas *_Canvas, l_num _Layer) {
	swapLayer(_Canvas, _Layer, _Layer + 1);
}
void _CANVAS_LAYER_ seekLayer(struct canvas *_Canvas, l_num _Layer) {
	swapLayer(_Canvas, _Layer, _Layer - 1);
}

void _CANVAS_DEBUG_ showLayer(struct canvas *_Canvas, l_num _Layer) {
	if(_Canvas == NULL) return;
	if(_Layer > _Canvas->layer || _Layer < 0) return;
	const c_size _LayerOffset = _Layer * _Canvas->block;
	register int _IPos, _JPos;
	putchar('\n');
	for(_JPos = 0; _JPos < _Canvas->height; ++_JPos) {
		for(_IPos = 0; _IPos < _Canvas->width; ++_IPos) {
			putchar(_Canvas->point[_LayerOffset + _IPos + _JPos * _Canvas->width]);
		}
		putchar('\n');
	}
}



#endif