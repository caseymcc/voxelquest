#version 330

$

layout(location=0) in vec4 vertexPos;
layout(location=1) in vec4 vertexTex;

out vec2 TexCoord0;

void main()
{
    TexCoord0=vertexTex.xy;
    gl_Position=vertexPos;
}

$

in vec2 TexCoord0;

layout(location=0) out vec4 FragColor0;

vec3 randv(vec2 co) {
    return vec3(
        fract(sin(dot(co.xy ,vec2(12.989, 78.233))) * 43758.8563),
        fract(sin(dot(co.yx ,vec2(93.989, 67.345))) * 24634.6345),
        fract(sin(dot(co.yx ,vec2(43.332, 93.532))) * 56445.2345)
    );
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	vec3 rv = randv(TexCoord0.xy);
	
	// float thresh = 0.65;	
	// rv.x *= float(rv.x>thresh);
	// rv.y *= float(rv.y>thresh);
	// rv.z *= float(rv.z>thresh);
	
    FragColor0 = vec4(rv,1.0);
    
}
