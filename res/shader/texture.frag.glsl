#version 330 core 
out vec4 frag_color;

in vec3 vColor;
in vec2 vTexCoord;

uniform sampler2D our_texture;

void main() {
	frag_color = texture(our_texture, TexCoord);
}
