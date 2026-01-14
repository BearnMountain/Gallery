#ifndef _MESH_H_
#define _MESH_H_

#include "defines.h"

typedef struct {
	f16 x, y, width, height;
} M_Rect; // rectangular bounds

typedef struct {
	u32 VAO;
	u32 VBO;
} Mesh; // stores each object rendered to the screen

Mesh* mesh_create(M_Rect* data);

#endif
