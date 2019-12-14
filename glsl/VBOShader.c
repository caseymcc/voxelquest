#version 330

uniform float heightOfNearPlane;
uniform float FOV;
uniform vec2 clipDist;
uniform vec2 bufferDim;

uniform mat4 pmMatrix;

$

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 data0;

void main() {
	
	vec4 screenPos = pmMatrix*vposition;

	gl_Position = screenPos;
}

$

layout(location = 0) out vec4 FragColor0;

void main() {

	FragColor0 = vec4(1.0, 1.0, 1.0, 1.0);

}