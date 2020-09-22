#version 400 core

#define PI 3.1415926535897932384626433832795

uniform sampler2D preProcess;
uniform vec2 invTexSize;
uniform float timeOffset;//Value from 0 to 2*PI

in vec2 texCo;

out vec4 FragColor;

void applyFXAA();

void main() {

	FragColor = texture2D(preProcess, texCo);

	//applyFXAA();

	/*//INVERTED COLORS
	FragColor = vec4(1.0 - texture2D(preProcess, texCo).xyz, 1.0);
	*/

	/*//TELEVISION INTERFERENCE
	int pixelHeight = 20;
	float offset = timeOffset / (2*PI);
	int divider = (int((texCo.y+offset) / (pixelHeight * invTexSize.y))%2==0 ? 1 : -1);
	vec2 newTexCo = vec2(texCo.x + sin(timeOffset*4) * pixelHeight * invTexSize.x * divider, texCo.y + pixelHeight * invTexSize.y * divider);
	FragColor = vec4(texture2D(preProcess, newTexCo).xyz, 1.0);
	*/

	/*//SHAKING
	float radius = 8, speed = -3.0;
	FragColor = vec4(texture2D(preProcess, vec2(texCo.x + radius*sin(timeOffset*speed)*invTexSize.x, texCo.y + radius*cos(timeOffset*speed)*invTexSize.y)).xyz, 1.0);
	*/

	/*//BLUR
	vec3 total;
	int blurRadius = 6, pixelChecks = 10;
	float angInc = 2*PI/pixelChecks;
	for(int i=0;i<pixelChecks;i++){
		total += texture2D(preProcess, vec2(texCo.x + sin(angInc*i)*blurRadius*invTexSize.x, texCo.y + cos(angInc*i)*blurRadius*invTexSize.y)).xyz;
	}
	total /= pixelChecks;
	FragColor = vec4(total, 1.0);
	*/

	/*//Double vision
	vec3 total;
	int blurSize = 20;
	for(int i=0;i<2;i++){
		for(int u=0;u<2;u++){
			
			total += texture2D(preProcess, vec2(texCo.x - (blurSize/2 - i*blurSize) * invTexSize.x, texCo.y - (blurSize/2 - u*blurSize) * invTexSize.y)).xyz;

		}
	}
	total /= 4.0;
	FragColor = vec4(total, 1.0);
	*/

	/*//PIXELATION EFFECT
	vec2 bounds = vec2(0.013, 0.013);
	vec3 color = texture2D(preProcess, vec2(int(texCo.x / bounds.x) * bounds.x, int(texCo.y / bounds.y) * bounds.y)).xyz;
	FragColor = vec4(color, 1.0);*/
	

}

void applyFXAA(){
	
	float FXAA_SPAN_MAX = 6000.0;
    float FXAA_REDUCE_MUL = 1.0/8.0;
    float FXAA_REDUCE_MIN = 1.0/256.0;

    vec3 rgbNW=texture2D(preProcess,texCo+(vec2(-1.0,-1.0)*invTexSize)).xyz;
    vec3 rgbNE=texture2D(preProcess,texCo+(vec2(1.0,-1.0)*invTexSize)).xyz;
    vec3 rgbSW=texture2D(preProcess,texCo+(vec2(-1.0,1.0)*invTexSize)).xyz;
    vec3 rgbSE=texture2D(preProcess,texCo+(vec2(1.0,1.0)*invTexSize)).xyz;
    vec3 rgbM=texture2D(preProcess,texCo).xyz;

    vec3 luma=vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
        FXAA_REDUCE_MIN);

    float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
          max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
          dir * rcpDirMin)) * invTexSize;

    vec3 rgbA = (1.0/2.0) * (
        texture2D(preProcess, texCo.xy + dir * (1.0/3.0 - 0.5)).xyz +
        texture2D(preProcess, texCo.xy + dir * (2.0/3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
        texture2D(preProcess, texCo.xy + dir * (0.0/3.0 - 0.5)).xyz +
        texture2D(preProcess, texCo.xy + dir * (3.0/3.0 - 0.5)).xyz);
    float lumaB = dot(rgbB, luma);

    if((lumaB < lumaMin) || (lumaB > lumaMax)){
        FragColor.xyz=rgbA;
    }else{
        FragColor.xyz=rgbB;
    }

}