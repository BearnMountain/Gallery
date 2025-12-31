#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "../../defines.h"
#include <OpenGL/gltypes.h>
#include <stddef.h>

typedef enum {
	WIDGET_LABEL,
	WIDGET_BUTTON,
	WIDGET_TEXTBOX,
	WIDGET_IMAGEBOX
} WidgetType;

typedef struct {
	f16 x, y, width, height;
} Rect;

typedef struct Widget {
	WidgetType type;
	Rect bounds;
	b32 visible;
	b32 enabled;
	void* data; // points to *Data struct
} Widget; 

// *Data struct
typedef struct {
	char text[256];
	f16 r,g,b,a; // text color
	f16 font_size;
} LabelData;
typedef struct {
	GLuint texture;
} ImageBoxData;
typedef struct {
	char text[256];
	size_t cursor_pos;
} TextBoxData;

void engine_init(void);

// hitbox
b32 widget_contains_bounds(Widget* widget, f16 x, f16 y);

#endif
