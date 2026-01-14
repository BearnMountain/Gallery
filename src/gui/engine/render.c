#include "render.h"
#include "log.h"
#include "util/io.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE // removes OpenGL/gl.h include, uses gl3.h
#include <GLFW/glfw3.h>

static u32 shader_program;

const char* VERTEX_SRC = "#version 330 core\nlayout (location = 0) in vec3 aPos;\nvoid main() {\ngl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n}"; 
const char* FRAGMENT_SRC = "#version 330 core\nout vec4 FragColor;\nvoid main() {\n	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n}";

f16 vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

void render_init(void) {

	// vertex
	// File vert_shader = io_file_read("res/shader/vertex.vert.glsl");
	u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &VERTEX_SRC, NULL);
	glCompileShader(vertex_shader);

	b32 success;
	char info_log[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		log_warn("render_init vertex shader failed to compile: %s", info_log);
		return;
	}

	// frag
	//File frag_shader = io_file_read("res/shader/fragment.frag.glsl");
	u32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &FRAGMENT_SRC, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		log_warn("render_init fragment shader failed to compile: %s", info_log);
	}

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		log_warn("render_init shader program failed to init: %s", info_log);
	}

	// cleanup
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void render_uninit(Mesh* meshes, const u32 count) {
	// remove opengl elements form the gpu
	for (u32 i = 0; i < count; i++) {
		glDeleteVertexArrays(1, &meshes[i].VAO);
		glDeleteVertexArrays(1, &meshes[i].VBO);
	}
}

void render_load_mesh(const float* vertices, const u32 triangle_count, Mesh* mesh) {
	glGenVertexArrays(1, &mesh->VAO);
	glBindVertexArray(mesh->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, 
			  triangle_count*3*6, // 3 vertices in a triangle * 6 values(x,y,r,g,b,a) per
			  vertices, GL_STATIC_DRAW
	);
	glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(1,4,GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}


void render(Mesh* meshes, u32 mesh_count) {
	glUseProgram(shader_program);
	for (u32 i = 0; i < mesh_count; i++) {
		glBindVertexArray(meshes[i].VAO);
	}
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
