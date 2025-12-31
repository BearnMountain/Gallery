#include <stdio.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "log.h"
#include "keybind.h"
#include "gui/gui.h"

void framebuffer_size_callback(GLFWwindow* window, i32 width, i32 height) {
	glViewport(0, 0, width, height);
}

int main(void) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// setup window
	GLFWwindow* window = glfwCreateWindow(config.window_width, config.window_height, "Gallery", NULL, NULL);
	if (window == NULL) {
		log_err("Failed to create glfw window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// setup glad
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		log_err("Failed to init GLAD");
		return -1;
	}

	// setup viewport
	glViewport(0, 0, config.window_width, config.window_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	

	// setup engine
	gui_init();

	// game loop
	while (!glfwWindowShouldClose(window)) {
		keybind_process_input(window);

		// run engine
		gui_render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	log("Closing");

	glfwTerminate();
	return 0;
}
