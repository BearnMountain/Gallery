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

// mesh values updated with VAO and VB
void render_load_mesh(Mesh* mesh, const f32* vertices, const u32 vertex_count, const u32* indices, const u32 indice_count);
void render_load_texture(Mesh* mesh, const char* path, const f32* vertices, const u32 vertex_count, const u32* indices, const u32 indice_count);
void render(Mesh* meshes, u32 mesh_count);

#endif
