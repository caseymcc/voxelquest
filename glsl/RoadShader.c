#version 330

uniform sampler2D Texture0; //cityFBO

uniform float mapStep;
uniform float texPitch;
uniform float doDilate;
uniform float seaLevel;

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
// bvec4 getGrid(int val) {

//     int testVal = val;

//     int hasDown = testVal / 8;
//     testVal -= hasDown * 8;
    
//     int hasUp = testVal / 4;
//     testVal -= hasUp*4;
    
//     int hasRight = testVal/2;
//     testVal -= hasRight*2;

//     int hasLeft = testVal;


//     return bvec4(
//         (hasLeft > 0),
//         (hasRight > 0),
//         (hasUp > 0),
//         (hasDown > 0)
//     );
// }

void main() {

    int i;


    float offsetAmount = 1.0/texPitch;
    float testHeight = 0.0;
    float heightMod = 0.0;
    float rand1 = 0.0;
    float rand2 = 0.0;
    float v1 = 0.0;
    float v2 = 0.0;
    float v3 = 0.0;

    vec4 baseTex = texture(Texture0, TexCoord0.xy);
    float tex1 = baseTex.b;
    float tex1r = texture(Texture0, vec2(TexCoord0.x + offsetAmount, TexCoord0.y) ).b;
    float tex1l = texture(Texture0, vec2(TexCoord0.x - offsetAmount, TexCoord0.y) ).b;
    float tex1u = texture(Texture0, vec2(TexCoord0.x, TexCoord0.y + offsetAmount) ).b;
    float tex1d = texture(Texture0, vec2(TexCoord0.x, TexCoord0.y - offsetAmount) ).b;

    float tot = 0.0;

    if (tex1 != tex1r) {
        tot += 1.0;
    }
    if (tex1 != tex1l) {
        tot += 2.0;
    }
    if (tex1 != tex1u) {
        tot += 4.0;
    }
    if (tex1 != tex1d) {
        tot += 8.0;
    }

    tot /= 255.0;

/*
    bvec4 highways = getGrid(tot);
    bvec4 streets = getGrid(baseTex.r);
    bvec4 alleys = getGrid(baseTex.g);
*/
    vec4 finalRes = baseTex;
    finalRes.b = tot;


    // vec4 tex1ul = texture(Texture0, vec2(TexCoord0.x - offsetAmount, TexCoord0.y + offsetAmount) );
    // vec4 tex1ur = texture(Texture0, vec2(TexCoord0.x + offsetAmount, TexCoord0.y + offsetAmount) );
    // vec4 tex1dl = texture(Texture0, vec2(TexCoord0.x - offsetAmount, TexCoord0.y - offsetAmount) );
    // vec4 tex1dr = texture(Texture0, vec2(TexCoord0.x + offsetAmount, TexCoord0.y - offsetAmount) );

    
    


    FragColor0 = finalRes;
}


