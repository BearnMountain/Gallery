#include "render.h"
#include "log.h"
#include "stb_image.h"

#include <glad/glad.h>
#include <stdio.h>
#define GLFW_INCLUDE_NONE // removes OpenGL/gl.h include, uses gl3.h
#include <GLFW/glfw3.h>

static u32 shader_program;

const char* VERTEX_SRC = "#version 330 core\nlayout (location = 0) in vec3 aPos;\nvoid main() {\ngl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n}"; 
const char* FRAGMENT_SRC = "#version 330 core\nout vec4 FragColor;\nvoid main() {\n	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n}";

f32 vertices[] = {
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
		glDeleteBuffers(1, &meshes[i].VBO);
		glDeleteBuffers(1, &meshes[i].EBO);
	}
}

void render_load_mesh(Mesh* mesh, 
					  const f32* vertices, const u32 vertex_count, 
					  const u32* indices, const u32 indice_count) {

	mesh->index_count = indice_count;
	
	glGenVertexArrays(1, &mesh->VAO);
	glGenBuffers(1, &mesh->VBO);
	glGenBuffers(1, &mesh->EBO);

	glBindVertexArray(mesh->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(
		GL_ARRAY_BUFFER,  
		vertex_count * 6 * sizeof(f32),
		vertices, 
		GL_STATIC_DRAW
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		indice_count * sizeof(u32),
		indices, 
		GL_STATIC_DRAW
	);

	// loads verts to shaders
	glVertexAttribPointer(
		0, 2, GL_FLOAT, GL_FALSE,
		6 * sizeof(f32), // 6 float per vertex: x,y,r,g,b,a
		(void*)0
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1, 4, GL_FLOAT, GL_FALSE, 
		6 * sizeof(f32), 
		(void*)(2 * sizeof(f32))
	);
	glEnableVertexAttribArray(1);


}

void render_load_texture(Mesh* mesh, const char* path,
						 const f32* vertices, const u32 vertice_count,
						 const u32* indices, const u32 indice_count) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	f32 border_color[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	// load texture file
	i32 image_width, image_height, image_nr_channels;
	unsigned char* data = stbi_load(path, &image_width, &image_height, &image_nr_channels, 0);

	if (!data) {
		log_warn("render_load_texture failed to load texture of path: %s", path);
		stbi_image_free(data);
		return;
	}

	// prepare texture
	glGenTextures(1, &mesh->texture);	
	glBindTexture(GL_TEXTURE_2D, mesh->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
		
	stbi_image_free(data);

	// setting up mesh	

	glVertexAttribPointer(
		2, 2, GL_FLOAT, GL_FALSE,
		7 * sizeof(f32),
		(void*)(5 * sizeof(f32))
	);
	glEnableVertexAttribArray(2);
}

void render(Mesh* meshes, u32 mesh_count) {
	glUseProgram(shader_program);
	for (u32 i = 0; i < mesh_count; i++) {
		glBindVertexArray(meshes[i].VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(
			GL_TRIANGLES,
			meshes[i].index_count,
			GL_UNSIGNED_INT,
			0
		);
	}
	glBindVertexArray(0);
}
