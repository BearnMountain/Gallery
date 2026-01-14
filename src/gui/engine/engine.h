#ifndef _ENGINE_H_
#define _ENGINE_H_

/*
	Engine knows what exists and how to create different widgets
*/

#include "defines.h"
#include "render.h"

#include <stddef.h>

typedef struct {
	u8 r, g, b, a;
} Color;

// Camera
typedef struct {
	f16 x, y;
} Camera;

void engine_init(Camera* scene_camera); // camera used for every engine render call
void engine_uninit(void);

// create: all return ids to the object
Mesh* engine_create_label(const char* text, const M_Rect bounds, const Color color, b32 visible); // returns an ID
Mesh* engine_create_image(void); // todo 
Mesh* engine_create_textbox(void); // todo
Mesh* engine_create_button(void); // todo


// delete: pass id
void engine_delete_widget(u32 id);


// render scene
void engine_render(void);

#endif
