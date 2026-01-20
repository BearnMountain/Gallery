#include "camera.h"

static f32 camera_x;
static f32 camera_y;
static f32 camera_zoom;

void camera_init(void) {
	camera_x = 0.0f;
	camera_y = 0.0f;
	camera_zoom = 0.0f;
}

void camera_move(const f32 x, const f32 y) {
	(void)x;
	(void)y;
}

void camera_set_zoom(const f32 zoom) {
	(void)zoom;	
}
