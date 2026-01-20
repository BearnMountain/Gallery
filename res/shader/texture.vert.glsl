#version 330
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vColor;
out vec2 vTexCoord;

void main() {
	gl_Position = vec4(aPos, 0.0f, 1.0f);
	vColor = aColor;
	vTexCoord = aTexCoord;
}
