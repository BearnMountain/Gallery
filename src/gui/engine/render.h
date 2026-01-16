#ifndef _RENDER_H_
#define _RENDER_H_

/*
	Knows how to render objects to the screen
	 - takes a que of widgets to be rendered, which ones are found to be 
	   inside the frame and renders them to the screen
	 - FILE ONLY MEANT FOR ENGINE.H
*/

#include "defines.h"
#include "mesh.h"

void render_init(void);
void render_uninit(Mesh* meshes, const u32 count);

// mesh values updated with VAO and VBO
void render_load_mesh(const f32* vertices, const u32 triangle_count, Mesh* mesh);
void render(Mesh* meshes, u32 mesh_count);

#endif
