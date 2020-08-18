#ifndef CANVAS_H
#define CANVAS_H

#include "core/_CORE_Canvas.h"


typedef struct canvas Canvas;
typedef struct graph Graph;

void _CANVAS_CURSOR_DRAWER_ setCursor(
	c_size						_X,
	c_size						_Y
);
void _CANVAS_CURSOR_DRAWER_ moveCursor(
	c_size						_DeltaX,
	c_size						_DeltaY
);
void _CANVAS_CURSOR_DRAWER_ drawChr(
	c_size						_X,
	c_size						_Y,
	p_chr						_Chr
);
p_chr _CANVAS_CURSOR_DRAWER_ absrChr(
	c_size						_X,
	c_size						_Y
);

int _CANVAS_BASIS_ isVisible(
	p_chr						_Chr
);
p_chr _CANVAS_BASIS_ topVisibleChr(
	Canvas*						_Canvas,
	c_size						_XPos,
	c_size						_YPos
);
l_num _CANVAS_BASIS_ topVisibleLayer(
	Canvas*						_Canvas,
	c_size						_XPos,
	c_size						_YPos
);
void _CANVAS_BASIS_ updateCanvas(
	Canvas*						_Canvas,
	c_size						_LeftPos,
	c_size						_RightPos,
	c_size						_TopPos,
	c_size						_BottomPos
);
Canvas* _CANVAS_BASIS_ createCanvasByPureChr(
	c_size						_CanvasWidth,
	c_size						_CanvasHeight,
	p_chr						_BackgroundChr
);
Graph _CANVAS_BASIS_ createGraph(
	c_size						_GraphWidth,
	c_size						_GraphHeight,
	p_chr*						src
);

Graph _CANVAS_FORMAT_DRAWER_ createGraphByModification(
	Graph						_Graph,
	p_chr						_OriginChr,
	p_chr						_TargetChr
);
Graph _CANVAS_FORMAT_DRAWER_ extractFormatGraphOnLayer(
	Canvas*						_Canvas,
	l_num						_Layer,
	c_size						_XPos,
	c_size						_YPos,
	c_size						_GraphWidth,
	c_size						_GraphHeight,
	p_chr						_FormatChr,
	p_chr*						_Format
);
void _CANVAS_FORMAT_DRAWER_ drawFormatGraphOnLayer(
	Canvas*						_Canvas,
	l_num						_Layer,
	Graph						_Graph,
	c_size						_XPos,
	c_size						_YPos,
	p_chr						_FormatChr,
	p_chr*						_Format
);
void _CANVAS_FORMAT_DRAWER_ eraseFormatGraphOnLayer(
	Canvas*						_Canvas,
	l_num						_Layer,
	c_size						_XPos,
	c_size						_YPos,
	c_size						_RectWidth,
	c_size						_RectHeight,
	p_chr						_FormatChr,
	p_chr*						_Format
);
void _CANVAS_RECT_DRAWER_ drawRectGraphOnTop(
	Canvas*						_Canvas,
	Graph						_Graph,
	c_size						_XPos,
	c_size						_YPos
);
Graph _CANVAS_RECT_DRAWER_ extractRectGraphOnLayer(
	Canvas*						_Canvas,
	l_num						_Layer,
	c_size						_XPos,
	c_size						_YPos,
	c_size						_RectWidth,
	c_size						_RectHeight
);
void _CANVAS_RECT_DRAWER_ drawRectGraphOnLayer(
	Canvas*						_Canvas,
	l_num						_Layer,
	Graph						_Graph,
	c_size						_XPos,
	c_size						_YPos,
	int							_Mode
);
void _CANVAS_RECT_DRAWER_ eraseRectGraphOnLayer(
	Canvas*						_Canvas,
	l_num						_Layer,
	c_size						_XPos,
	c_size						_YPos,
	c_size						_RectWidth,
	c_size						_RectHeight
);

void _CANVAS_LAYER_ removeLayer(
	Canvas*						_Canvas,
	l_num						_Layer
);
void _CANVAS_LAYER_ moveLayer(
	Canvas*						_Canvas,
	l_num						_Layer,
	c_size						_DeltaX,
	c_size						_DeltaY
);
void _CANVAS_LAYER_ changeLayDspl(
	Canvas*						_Canvas,
	l_num						_Layer
);
void _CANVAS_LAYER_ combineLayer(
	Canvas*						_Canvas,
	l_num						_DestLayer,
	l_num						_SrcLayer,
	int							_Mode
);
void _CANVAS_LAYER_ swapLayer(
	Canvas*						_Canvas,
	l_num						_Layer1,
	l_num						_Layer2
);
void _CANVAS_LAYER_ flowLayer(
	Canvas*						_Canvas,
	l_num						_Layer
);
void _CANVAS_LAYER_ seekLayer(
	Canvas*						_Canvas,
	l_num						_Layer
);

Graph _CANVAS_IOSTREAM_ importGraphFromCANVGFile(
	const char*					_InStreamPath
);
void _CANVAS_IOSTREAM_ exportGraphToCANVGFile(
	const char*					_OutStreamPath,
	Graph						_Graph
);
void _CANVAS_IOSTREAM_ exportLayerToCANVGFILE(
	const char*					_OutStreamPath,
	Canvas*						_Canvas,
	l_num						_Layer,
	int							mode
);

#endif