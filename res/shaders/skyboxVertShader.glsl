#version 460 core

in vec3 position;
out vec3 texture_coords;

uniform mat4 proj;
uniform mat4 view;

void main(void){
	
	gl_Position = proj * view * vec4(position, 1.0); 
	texture_coords = position;
	
}