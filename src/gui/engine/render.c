#include "render.h"
#include "log.h"
#include "stb_image.h"
#include "util/io.h"

#include <glad/glad.h>
#include <stdio.h>
#define GLFW_INCLUDE_NONE // removes OpenGL/gl.h include, uses gl3.h
#include <GLFW/glfw3.h>

static u32 mesh_shader_program;
static u32 texture_shader_program;

f32 vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

void load_shaders(u32* shader_program, const char* vertex_src, const char* fragment_src) {
	// vertex
	u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_src, NULL);
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
	u32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_src, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		log_warn("render_init fragment shader failed to compile: %s", info_log);
		return;
	}

	*shader_program = glCreateProgram();
	glAttachShader(*shader_program, vertex_shader);
	glAttachShader(*shader_program, fragment_shader);
	glLinkProgram(*shader_program);

	glGetProgramiv(*shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(*shader_program, 512, NULL, info_log);
		log_warn("render_init shader program failed to init: %s", info_log);
		return;
	}

	// cleanup
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void render_init(void) {
	File vert = io_file_read("res/shader/obj.vert.glsl");
	File frag = io_file_read("res/shader/obj.frag.glsl");

	load_shaders(&mesh_shader_program, vert.data, frag.data);
	free(vert.data);
	free(frag.data);

	vert = io_file_read("res/shader/texture.vert.glsl");
	frag = io_file_read("res/shader/texture.frag.glsl");

	load_shaders(&texture_shader_program, vert.data, frag.data);
	free(vert.data);
	free(frag.data);
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
						 const f32* vertices, const u32 vertex_count,
						 const u32* indices, const u32 indice_count) {

	mesh->index_count = indice_count;

	// load texture file
	i32 image_width, image_height, image_nr_channels;
	stbi_set_flip_vertically_on_load(1); // for opengl
	unsigned char* data = stbi_load(path, &image_width, &image_height, &image_nr_channels, 0);

	if (!data) {
		log_warn("render_load_texture failed to load texture of path: %s", path);
		stbi_image_free(data);
		return;
	}

	// prepare texture
	glGenTextures(1, &mesh->texture);	
	glBindTexture(GL_TEXTURE_2D, mesh->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f32 border_color[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
    	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, (image_nr_channels == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
		
	stbi_image_free(data);

	// setting up mesh VAO, VBO, EBO	
	glGenVertexArrays(1, &mesh->VAO);
	glGenBuffers(1, &mesh->VBO);
	glGenBuffers(1, &mesh->EBO);

	glBindVertexArray(mesh->VAO);

	// load vertex data
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertex_count * 7 * sizeof(f32),
		vertices, 
		GL_STATIC_DRAW
	);
	
	// load ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indice_count * sizeof(u32),
		indices, 
		GL_STATIC_DRAW
	);

	// load attributes
	glVertexAttribPointer(
		0, 2, GL_FLOAT, GL_FALSE,
		7 * sizeof(f32),
		(void*)0
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE, // just RGB
		7 * sizeof(f32),
		(void*)(2 * sizeof(f32))
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2, 2, GL_FLOAT, GL_FALSE,
		7 * sizeof(f32),
		(void*)(5 * sizeof(f32))
	);
	glEnableVertexAttribArray(2);
}

void render(Mesh* meshes, u32 mesh_count) {
	for (u32 i = 0; i < mesh_count; i++) {
		if (meshes[i].texture) {
			glUseProgram(texture_shader_program);

			glBindTexture(GL_TEXTURE_2D, meshes[i].texture);
			glBindVertexArray(meshes[i].VAO);
			glDrawElements(
				GL_TRIANGLES,
				meshes[i].index_count,
				GL_UNSIGNED_INT,
				0
			);
		} else {
			glUseProgram(mesh_shader_program);

			glBindVertexArray(meshes[i].VAO);
			glDrawElements(
				GL_TRIANGLES,
				meshes[i].index_count,
				GL_UNSIGNED_INT,
				0
			);
		}
	}
	glBindVertexArray(0);
}
