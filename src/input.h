#ifndef _INPUT_H_
#define _INPUT_H_

#include "GLFW/glfw3.h"

typedef enum {
    ACTION_MOVE_UP,
    ACTION_QUIT,
    ACTION_COUNT
} Action;

void input_process_input(GLFWwindow* window);
void input_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void input_mouse_callback(GLFWwindow* window, double x, double y);

#endif
