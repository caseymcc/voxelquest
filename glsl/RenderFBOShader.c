#version 330

////////////////  Render  ////////////////

uniform sampler2D Texture0;

$
out vec2 texCoord;

layout(location=0) in vec3 vertexPos;
layout(location=1) in vec2 vertexTex;

void main()
{
    texCoord=vertexTex;
    gl_Position=vec4(vertexPos, 1.0);
}
$

in vec2 texCoord;
out vec4 color;

void main()
{
    color=texture(Texture0, texCoord);
}