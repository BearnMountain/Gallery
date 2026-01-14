#include <stdio.h>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE // removes OpenGL/gl.h include, uses gl3.h
#include <GLFW/glfw3.h>

#include "config.h"
#include "log.h"
#include "input.h"
#include "gui/gui.h"

void framebuffer_size_callback(GLFWwindow* window, i32 width, i32 height) {
	(void)window;
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		log_err("Failed to init GLAD");
		return -1;
	}
	
	// setup viewport
	glViewport(0, 0, config.window_width, config.window_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	

	// setup engine
	gui_init();

	// setup keybind callback
	// glfwSetKeyCallback(window, keybind_callback);
	// glfwSetCharCallback(GLFWwindow *window, GLFWcharfun callback)

	// game loop
	while (!glfwWindowShouldClose(window)) {
		// keybind_process_input(window);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

		// run engine
		gui_render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	log("Closing");

	glfwTerminate();
	return 0;
}
