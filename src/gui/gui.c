#include "gui.h"
#include <OpenGL/gl3.h>
#include <string.h>

#include "defines.h"
#include "engine/engine.h"
#include "log.h"

static Camera camera;

void gui_init(void) {
	camera.x = 0;
	camera.y = 0;

	engine_init(&camera);

	// setting the scene
	engine_create_label("test", &(M_Rect){100,100,100,100}, &(Color){100,100,100,255}, true);
	engine_create_image("res/textures/tests.png", &(M_Rect){300,100,100,100}, false);

}

void gui_render(void) {
	// set background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// calls engine to render everything it passes
	engine_render();
}
