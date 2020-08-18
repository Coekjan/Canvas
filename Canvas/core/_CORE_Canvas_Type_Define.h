#ifndef INNER_CANVAS_CANVAS_TYPE_DEFINE_H
#define INNER_CANVAS_CANVAS_TYPE_DEFINE_H

#ifndef CANVAS_HEAD_PATH
#define CANVAS_HEAD_PATH "Canvas"
#endif

#define _CANVAS_DEBUG_
#define _CANVAS_LAYER_
#define _CANVAS_BASIS_
#define _CANVAS_CURSOR_DRAWER_
#define _CANVAS_RECT_DRAWER_
#define _CANVAS_FORMAT_DRAWER_
#define _CANVAS_IOSTREAM_
#define _CANVAS_SYMBOL_IOSTREAM_

#define CANVAS_CANVG_FORMAT_IN "%[A-Z]_%d_%d"
#define CANVAS_CANVG_FORMAT_OUT "%s_%d_%d"
#define CANVAS_CANVG_FORMAT_HEAD "CANVG"
#define CANVAS_CANVG_FORMAT_HEAD_LENGTH 6
#define CANVAS_LAYER_MAX 0xFF
#define CANVAS_LAYER_DRAW_COVER 0xFF
#define CANVAS_LAYER_DRAW_SKIP 0x0
#define CANVAS_LAYER_EXPORT_WHOLE 0xFF
#define CANVAS_LAYER_EXPORT_FIT 0x0
#define GRAPH_CHR_NULL ((p_chr*)0)
#define GRAPH_NULL ((struct graph){0, 0, GRAPH_CHR_NULL})
#define LAYER_NULL ((l_num)-1)

#define CANVAS_CANVG ".canvg"
#define CANVAS_SYMBOL_PATH CANVAS_HEAD_PATH "/baseSym/"

#define CANVAS_COUNT_NUMBER 10
#define CANVAS_NUMBER_PATH CANVAS_SYMBOL_PATH "_BASESYM_Number/"
#define CANVAS_NUMBER(_Font, _Number, _Width_X_Height) CANVAS_NUMBER_PATH #_Font "/" #_Number " " #_Width_X_Height CANVAS_CANVG

#define CANVAS_ALPHA_PATH CANVAS_SYMBOL_PATH "_BASESYM_Alpha/"
#define CANVAS_ALPHA(_Font, _Alpha, _Width_X_Height) CANVAS_ALPHA_PATH #_Font "/" #_Alpha " " #_Width_X_Height CANVAS_CANVG

typedef int c_size;
typedef int l_num;
typedef char p_chr;

struct canvas {
	c_size			width;
	c_size			height;
	c_size			block;
	l_num			layer;
	p_chr			bgchr;
	p_chr*			point;
	struct rect*	area;
};

struct graph {
	c_size			width;
	c_size			height;
	p_chr*			chr;
};

struct rect {
	c_size			left;
	c_size			right;
	c_size			top;
	c_size			bottom;
};

#endif