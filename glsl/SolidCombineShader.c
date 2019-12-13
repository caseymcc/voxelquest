#version 330
//#version 120

uniform bool skippedPrim;

//solidBaseTargFBO
uniform sampler2D Texture0;
uniform sampler2D Texture1;

//geomTargFBO
uniform sampler2D Texture2;
uniform sampler2D Texture3;

^INCLUDE:MATERIALS^

$

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 vtexcoord;

out vec2 TexCoord0;

void main() {

    TexCoord0 = vtexcoord.xy;
    gl_Position = vposition;
}

$

in vec2 TexCoord0;

layout(location = 0) out vec4 FragColor0;
layout(location = 1) out vec4 FragColor1;

void main() {
    vec4 tex0 = texture(Texture0, TexCoord0.xy);
    vec4 tex1 = texture(Texture1, TexCoord0.xy);
    vec4 tex2 = texture(Texture2, TexCoord0.xy);
    vec4 tex3 = texture(Texture3, TexCoord0.xy);
    
    // vec2 curTex = vec2(TEX_EARTH,0.5);
    // float curMat = floor(curTex.x*256.0*255.0) + floor(curTex.y*255.0);
    
    //tex3.w = curMat;
    
    if (skippedPrim) {
        
    }
    else {
        if (tex2.w > tex0.w) {
            tex0 = tex2;
            tex1 = tex3;
        }
    }
    
    
    
//    gl_FragData[0] = tex0;
//    gl_FragData[1] = tex1;
    FragColor0 = tex0;
    FragColor1 = tex1;
}


