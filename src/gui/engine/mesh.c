#include "mesh.h"
#include <stdlib.h>

Mesh* mesh_create(M_Rect* data) {
	Mesh* m = malloc(sizeof(Mesh));

	(void)data;
	
	return m;
}
