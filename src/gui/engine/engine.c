#include "engine.h"
#include "config.h"
#include "log.h"
#include <stdlib.h>
#include <glad/glad.h>

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
	f16 font_size;
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
	if (camera == NULL) {
		log_err("engine_init camera must not be null");
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
Mesh* engine_create_label(const char* text, const M_Rect bounds, const Color color, b32 visible) {
	(void)visible;
	(void)text;
	// TODO; bounds check?

	// resize mesh for more objects
	if (mesh_capacity == mesh_count) {
		mesh_capacity *= 2;
		meshes = realloc(meshes, mesh_capacity * sizeof(Mesh*));
	}
	
	// normalize data
	f16 x = (bounds.x / config.window_width)*2-1;
	f16 y = (bounds.y / config.window_height)*2-1;
	f16 width = (bounds.width / config.window_width)*2-1;
	f16 height = (bounds.height / config.window_height)*2-1;
	f16 r = color.r / 255.0f;
	f16 g = color.g / 255.0f;
	f16 b = color.b / 255.0f;
	f16 a = color.a / 255.0f;

	// create mesh vertices
	f16 rect[] = {
		// triangle 1
		x, y, COLOR,
		x + width, y, COLOR,
		x + width, y + height, COLOR,

		// triangle 2
		x, y, COLOR,
		x, y + height, COLOR,
		x + width, y + width, COLOR
	};

	// adding the mesh
	render_load_mesh(rect, 2, &meshes[mesh_count]);

	mesh_count++;
	return &meshes[mesh_count-1];
}

// b32 widget_contains_bounds(Widget* widget, f16 mouse_x, f16 mouse_y) {
// 	if (!widget->visible || !widget->enabled) return 0;
//     return mouse_x >= widget->bounds.x && mouse_x <= widget->bounds.x + widget->bounds.width &&
//            mouse_y >= widget->bounds.y && mouse_y <= widget->bounds.y + widget->bounds.height;
// }


