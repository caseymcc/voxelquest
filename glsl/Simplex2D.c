#version 330
////////////////  Simplex2D  ////////////////



//varying vec2 TexCoord0;
uniform vec2 resolution;
uniform float curTime;

$
layout(location=0) in vec4 vertexPos;
layout(location=1) in vec4 vertexTex;

out vec2 texCoord;

void main() {
    texCoord=vertexTex.xy;
    gl_Position=vec4(vertexPos.xyz, 1.0);
}

//void main() {
//    TexCoord0 = gl_MultiTexCoord0.xy;
//    gl_Position = gl_Vertex;
//}

$

in vec2 texCoord;
out vec4 color;

//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


vec2 mod289(vec2 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}
vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
    return mod289(((x*34.0)+1.0)*x);
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec2 v) {
    const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
    // First corner
    vec2 i  = floor(v + dot(v, C.yy) );
    vec2 x0 = v -   i + dot(i, C.xx);

    // Other corners
    vec2 i1;
    //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
    //i1.y = 1.0 - i1.x;
    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    // x0 = x0 - 0.0 + 0.0 * C.xx ;
    // x1 = x0 - i1 + 1.0 * C.xx ;
    // x2 = x0 - 1.0 + 2.0 * C.xx ;
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;

    // Permutations
    i = mod289(i); // Avoid truncation effects in permutation
    vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
        + i.x + vec3(0.0, i1.x, 1.0 ));

    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
    m = m*m ;
    m = m*m ;

    // Gradients: 41 points uniformly over a line, mapped onto a diamond.
    // The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;

    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt( a0*a0 + h*h );
    m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

    // Compute final noise value at P
    vec3 g;
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}




//%%%%%%
//
// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 



float snoise(vec3 v) { 
    const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
    const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

    // First corner
    vec3 i  = floor(v + dot(v, C.yyy) );
    vec3 x0 =   v - i + dot(i, C.xxx) ;

    // Other corners
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min( g.xyz, l.zxy );
    vec3 i2 = max( g.xyz, l.zxy );

    //   x0 = x0 - 0.0 + 0.0 * C.xxx;
    //   x1 = x0 - i1  + 1.0 * C.xxx;
    //   x2 = x0 - i2  + 2.0 * C.xxx;
    //   x3 = x0 - 1.0 + 3.0 * C.xxx;
    vec3 x1 = x0 - i1 + C.xxx;
    vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
    vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

    // Permutations
    i = mod289(i); 
    vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

    // Gradients: 7x7 points over a square, mapped onto an octahedron.
    // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
    float n_ = 0.142857142857; // 1.0/7.0
    vec3  ns = n_ * D.wyz - D.xzx;

    vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

    vec4 x = x_ *ns.x + ns.yyyy;
    vec4 y = y_ *ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);

    vec4 b0 = vec4( x.xy, y.xy );
    vec4 b1 = vec4( x.zw, y.zw );

    //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
    //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
    vec4 s0 = floor(b0)*2.0 + 1.0;
    vec4 s1 = floor(b1)*2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));

    vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
    vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

    vec3 p0 = vec3(a0.xy,h.x);
    vec3 p1 = vec3(a0.zw,h.y);
    vec3 p2 = vec3(a1.xy,h.z);
    vec3 p3 = vec3(a1.zw,h.w);

    //Normalise gradients
    vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;

    // Mix final noise value
    vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
    m = m * m;
    return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3) ) );
}
//%%%%%%






float calcNoise(vec3 uvw) {
    float baseScale = 1.0;
    float n = 0.0;
    n += snoise(uvw * baseScale);
    n += 0.5 * snoise(uvw * baseScale*2.0);


    
    //n += 0.25 * snoise(uvw * baseScale*4.0);
    //n += 0.125 * snoise(uvw * baseScale*8.0);
    
    /*
    n += 0.0625 * snoise(uvw * baseScale*16.0);
    n += 0.03125 * snoise(uvw * baseScale*32.0);
    n += 0.015625 * snoise(uvw * baseScale*64.0);
    n += 0.0078125 * snoise(uvw * baseScale*128.0);
    */    

    
    //n = (n + 1.0)/2.0;
    //n = clamp(n*1.3,0.0,1.0);
    //n = pow(n,4.0);

    return n;
}

float caclNoiseSL(vec3 uvw) {
    float w = 1.0;
    float h = 1.0;

    float x = uvw.x;
    float y = uvw.y;
    float z = uvw.z;

    return (
        calcNoise( vec3(x, y, z) ) * (w - x) * (h - y) +
        calcNoise( vec3(x - w, y, z) ) * (x) * (h - y) +
        calcNoise( vec3(x - w, y - h, z) ) * (x) * (y) +
        calcNoise( vec3(x, y - h, z) ) * (w - x) * (y)
    ) / (w * h);
} 

void main() {
    
//    vec3 v_texCoord3D = vec3(TexCoord0.x,TexCoord0.y,0.5);
//    //float newTime = curTime/5000.0;
//    vec3 uvw = vec3(TexCoord0.xy,curTime);
    vec3 v_texCoord3D=vec3(texCoord.x, texCoord.y, 0.5);
    vec3 uvw=vec3(texCoord.xy, curTime);

    
    float n = 0.0;
    int i;

    vec4 res = vec4(0.0);

    for (i = 0; i < 3; i++) {
            

            n = caclNoiseSL(uvw);

            if (i == 0) {
                uvw.z += 37.0;
                res.r = n;
            }
            if (i == 1) {
                uvw.z += 21.0;
                res.g = n;
            }
            if (i == 2) {
                res.b = n;
            }
    }

    res.rgb = normalize(res.rgb);

    res.rgb = (res.rgb + 1.0)/2.0;
    
    
//    gl_FragData[0] = res;//vec4(lerpAmount);//res;
    color=res;
}

/*
void main() {
    
    vec3 v_texCoord3D = vec3(TexCoord0.x,TexCoord0.y,0.5);

    float newTime = curTime/5000.0;

    // Perturb the texcoords with three components of noise
    vec3 uvw = v_texCoord3D;
    float baseScale = 2.0;
    float n = 0.0;


    
    n += snoise(uvw*baseScale - vec3(0.0, 0.0, newTime));
    n += 0.5 * snoise(uvw * baseScale*2.0 - vec3(0.0, 0.0, newTime*1.4)); 
    n += 0.25 * snoise(uvw * baseScale*4.0 - vec3(0.0, 0.0, newTime*2.0));
    n += 0.125 * snoise(uvw * baseScale*8.0 - vec3(0.0, 0.0, newTime*2.8)); 
    n += 0.0625 * snoise(uvw * baseScale*16.0 - vec3(0.0, 0.0, newTime*4.0)); 
    n += 0.03125 * snoise(uvw * baseScale*32.0 - vec3(0.0, 0.0, newTime*5.6));
    n += 0.015625 * snoise(uvw * baseScale*64.0 - vec3(0.0, 0.0, newTime*7.3));
    n += 0.0078125 * snoise(uvw * baseScale*128.0 - vec3(0.0, 0.0, newTime*8.9)); 
    

    n = (n + 1.0)/2.0;
    

    gl_FragData[0] = vec4(n);
}
*/