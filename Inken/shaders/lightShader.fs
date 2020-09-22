#version 400 core

#define MAX_LIGHTS			256
#define AMBIENT_LIGHT		1.0//0.1
#define OCCLUSION			0.2
#define SHININESS			0.2

layout (location = 0) out vec4 outputTex;

in vec2 TexCoords;

uniform sampler2D gPosition, gNormal, gColor;

uniform vec3 camPos;
uniform vec4 ambientColor;

uniform vec4 lightColor[MAX_LIGHTS], lightPos[MAX_LIGHTS];
uniform int lightSize;

void main(){
	
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec4 TexColor = texture(gColor, TexCoords);

	if(length(FragPos) == 0){
		outputTex = ambientColor;
		return;
	}

	if(length(Normal) == 0.0){//Presumably a graphic
		//outputTex = vec4(TexColor, length(TexColor));
		outputTex = TexColor;
		return;
	}

	vec3 viewDir = normalize(camPos - FragPos);

	vec3 lightTotal;
	for(int i=0;i<lightSize;i++){
		
		vec3 dirToLight = normalize(lightPos[i].xyz - FragPos);
		float dist = distance(FragPos, lightPos[i].xyz);
		if(dist > lightPos[i].w){//Check for range bounds
			continue;
		}

		vec3 reflectDir = reflect(-dirToLight, Normal);
		float specSheen = pow(max(dot(viewDir, reflectDir), 0.0), 16);
		float specular = max(dot(dirToLight, Normal), 0.2);

		float distMul = pow(max(0.0, 1.0 - dist / lightPos[i].w), OCCLUSION);

		lightTotal += lightColor[i].xyz * lightColor[i].w * distMul * specular * (1.0 + specSheen * SHININESS); //Color*Intensity*DistanceMultiplier*Specular*Sheen
		
	}

	lightTotal = max(lightTotal, vec3(AMBIENT_LIGHT));

	outputTex = vec4(TexColor.xyz * lightTotal, TexColor.a);

}