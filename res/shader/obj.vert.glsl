#version 330 core 

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aClr;

out vec4 vClr;

void main() {
	gl_Position = vec4(aPos.xy, 0.0f, 1.0f);
	vClr = aClr;
}
