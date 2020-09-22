#version 400 core

#define MAX_TEXTURES	16

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
flat in int TexID;

uniform sampler2D textures[MAX_TEXTURES];
uniform vec3 tint;
uniform bool gui;

void main(){
	
	vec4 origColor = texture(textures[TexID], TexCoords);
	if(origColor.a < 1){
		discard;
	}

	gPosition = FragPos;
	gNormal = gui ? vec3(0.0) : normalize(Normal);
	gColor = vec4(origColor.xyz * tint, origColor.a);
	
}
