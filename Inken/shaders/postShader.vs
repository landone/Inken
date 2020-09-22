#version 400 core

layout (location = 0) in vec3 position;

out vec2 texCo;

void main(){
	
	texCo = vec2((position.x + 1)/2.0, (position.y + 1)/2.0);
	gl_Position = vec4(position.x, position.y, 0.0, 1.0);
	
}