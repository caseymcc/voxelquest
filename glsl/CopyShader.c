#version 330

uniform sampler2D Texture0;


$
layout(location=0) in vec4 vertexPos;
layout(location=1) in vec4 vertexTex;

out vec2 TexCoord0;

void main() {

    TexCoord0=vertexTex.xy;
    gl_Position=vertexPos;
}

$

in vec2 TexCoord0;
out vec4 FragColor0;

void main() {
    FragColor0=texture(Texture0, TexCoord0.xy);
}


