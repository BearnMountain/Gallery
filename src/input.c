#include "input.h"
#include "defines.h"
#include <stdio.h>

static unsigned char key_down[GLFW_KEY_LAST+1];

void input_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key < 0) return;
	(void)scancode;
	(void)mods;
	(void)window;

	// mod | GLFW_MOD_SHIFT etc for alt, control, super

	if (action == GLFW_PRESS) {
		key_down[key] = 1;
	} else if (action == GLFW_RELEASE) {
		key_down[key] = 0;
	} 

}

void input_mouse_callback(GLFWwindow* window, double x, double y) {
	(void)window;
	(void)x;
	(void)y;
}

void input_process_input(GLFWwindow* window) {
	if (key_down[GLFW_KEY_ESCAPE]) {
		glfwSetWindowShouldClose(window, true);
	}
}
