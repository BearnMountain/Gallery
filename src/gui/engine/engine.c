#include "engine.h"
#include "config.h"
#include "log.h"
#include <stdlib.h>
#include <glad/glad.h>
#include "stb_image.h"

#define COLOR r,g,b,a

typedef enum {
	WIDGET_LABEL,
	WIDGET_BUTTON,
	WIDGET_TEXTBOX,
	WIDGET_IMAGEBOX
} WidgetType;

typedef struct Widget {
	WidgetType type;
	M_Rect bounds;
	b32 visible;
	b32 enabled;
	void* data; // points to *Data struct
} Widget; 

// *Data struct
typedef struct {
	char text[256];
	u8 r,g,b,a; // text color
	f32 font_size;
} LabelData;
typedef struct {
	u32 textures;
} ImageBoxData;
typedef struct {
	char text[256];
	size_t cursor_pos;
} TextBoxData;
typedef struct {
	char text[256];
	void (*callback);
} ButtonData;

static Camera* camera; // camera must not be null
static Mesh* meshes;
static u32 mesh_capacity;
static u32 mesh_count;

void engine_init(Camera* scene_camera) {
	if (scene_camera == NULL) {
		log_warn("engine_init camera must not be null");
	}

	camera = scene_camera;
	meshes = malloc(sizeof(Mesh) * 8);
	mesh_capacity = 8;
	mesh_count = 0;

	render_init();
}

void engine_uninit(void) {
	// free gpu memory
	render_uninit(meshes, mesh_count);
}

// render loop
void engine_render(void) {
	// all work given to render class to deal with rendering pipeline
	render(meshes, mesh_count);
}

// creating widgets 
Mesh* engine_create_label(const char* text, const M_Rect* bounds, const Color* color, b32 visible) {
	(void)visible;
	(void)text;
	// TODO; bounds check?

	// resize mesh for more objects
	if (mesh_capacity == mesh_count) {
		mesh_capacity *= 2;
		meshes = realloc(meshes, mesh_capacity * sizeof(Mesh));
	}
	
	// normalize data
	f32 x1 = ((f32)bounds->x / config.window_width) * 2.0f - 1.0f;
	f32 y1 = ((f32)bounds->y / config.window_height) * 2.0f - 1.0f;
	f32 x2 = ((f32)(bounds->x+bounds->width) / config.window_width) * 2.0f - 1.0f;
	f32 y2 = ((f32)(bounds->y+bounds->height) / config.window_height) * 2.0f - 1.0f;
	f32 r = color->r / 255.0f;
	f32 g = color->g / 255.0f;
	f32 b = color->b / 255.0f;
	f32 a = color->a / 255.0f;

	// create mesh vertices and triangles 2 triangles with shared verts
	f32 rect[] = {
		x1, y1, COLOR,
		x2, y1, COLOR,
		x1, y2, COLOR,
		x2, y2, COLOR
	};
	
	u32 indices[] = {
		0, 1, 3,
		3, 2, 0
	};

	// adding the mesh
	render_load_mesh(&meshes[mesh_count], rect, 4, indices, 6);
	meshes[mesh_count].texture = 0;

	mesh_count++;
	return &meshes[mesh_count-1];
}

Mesh* engine_create_image(const char* path, const M_Rect* bounds, b32 visible) {
	(void)visible;

	if (mesh_capacity == mesh_count) {
		mesh_capacity *= 2;
		meshes = realloc(meshes, mesh_capacity * sizeof(Mesh));
	}

	f32 x1 = ((f32)bounds->x / config.window_width) * 2.0f - 1.0f;
	f32 y1 = ((f32)bounds->y / config.window_height) * 2.0f - 1.0f;
	f32 x2 = ((f32)(bounds->x+bounds->width) / config.window_width) * 2.0f - 1.0f;
	f32 y2 = ((f32)(bounds->y+bounds->height) / config.window_height) * 2.0f - 1.0f;

	(void)x1;(void)y1;(void)x2;(void)y2;

	float vertices[] = {
		// positions          // colors           // texture coords
		x2, y1,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		x2, y2,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		x1, y2,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		x1, y1,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	u32 indices[] = {
		0, 1, 2, 
		3, 2, 0
	}; 

	render_load_texture(&meshes[mesh_count], path, vertices, 4, indices, 6);

	mesh_count++;
	return &meshes[mesh_count-1];
}

Mesh* engine_create_textbox(void) {

	return NULL;
}

Mesh* engine_create_button(void) {

	return NULL;
}

b32 engine_remove_widget(const Mesh* mesh) {
	(void)mesh;
	log_warn("engine_remoce_widget");
	return false;
}
