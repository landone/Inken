#version 400 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texMap;
uniform vec3 tint;

void main(){
	
	vec4 origColor = texture(texMap, TexCoords);
	if(origColor.a < 1){
		discard;
	}
	FragColor = vec4(origColor.xyz * tint, 1);
	
}
