uniform sampler2D Texture0; // pagesFBO
uniform sampler2D Texture1;

uniform sampler2D Texture2; // geom fbo
uniform sampler2D Texture3;

varying vec2 TexCoord0;

$

void main() {
    TexCoord0 = gl_MultiTexCoord0.xy;
    gl_Position = gl_Vertex;
}

$

void main() {

    // pages
    vec4 tex0 = texture2D(Texture0, TexCoord0 );
    vec4 tex1 = texture2D(Texture1, TexCoord0 );

    // geom
    vec4 tex2 = texture2D(Texture2, TexCoord0 );
    vec4 tex3 = texture2D(Texture3, TexCoord0 );


    vec4 res0 = vec4(0.0);
    vec4 res1 = vec4(0.0);
    
    if (tex2.w > tex0.w) {
        res0 = tex2;
        res1 = tex3;
    }
    else {
        res0 = tex0;
        res1 = tex1;
    }
    
    gl_FragData[0] = res0;
    gl_FragData[1] = res1;


}
