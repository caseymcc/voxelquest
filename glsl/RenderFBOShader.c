#version 330

////////////////  Render  ////////////////

uniform sampler2D Texture0;

$
out vec2 texCoord;

layout(location=0) in vec4 vertexPos;
layout(location=1) in vec4 vertexTex;

void main()
{
    texCoord=vertexTex.xy;
    gl_Position=vertexPos;
}
$

in vec2 texCoord;
out vec4 color;

void main()
{
    color=texture(Texture0, texCoord);
}