#version 330
// Adapted from:
// http://callumhay.blogspot.com/2010/09/gaussian-blur-shader-glsl.html


uniform sampler2D Texture0;
uniform float sigma;     // The sigma value for the gaussian function: higher value means more blur
                         // A good value for 9x9 is around 3 to 5
                         // A good value for 7x7 is around 2.5 to 4
                         // A good value for 5x5 is around 2 to 3.5
                         // ... play around with this based on what you need :)

uniform float blurSize;  // This should usually be equal to
                         // 1.0 / texture_pixel_width for a horizontal blur, and
                         // 1.0 / texture_pixel_height for a vertical blur.

// kernel is this val * 2 + 1
uniform float numBlurPixelsPerSide;
// direction vector
uniform vec2 blurMultiplyVec;

const float pi = 3.14159265;

$
layout(location=0) in vec4 vertexPos;
layout(location=1) in vec4 vertexTex;

out vec2 v_texCoord;
out float newBlurSize;

void main()
{
    gl_Position =vertexPos;
    v_texCoord =vertexTex.xy;

    newBlurSize = blurSize;
}

$

in vec2 v_texCoord;
in float newBlurSize;

layout(location=0) out vec4 FragColor0;

void main() {

  // Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
  vec3 incrementalGaussian;
  incrementalGaussian.x = 1.0 / (sqrt(2.0 * pi) * sigma);
  incrementalGaussian.y = exp(-0.5 / (sigma * sigma));
  incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;

  vec4 avgValue = vec4(0.0, 0.0, 0.0, 0.0);
  float coefficientSum = 0.0;

  // Take the central sample first...
  avgValue += texture(Texture0, v_texCoord.xy) * incrementalGaussian.x;
  coefficientSum += incrementalGaussian.x;
  incrementalGaussian.xy *= incrementalGaussian.yz;

  // Go through the remaining 8 vertical samples (4 on each side of the center)
  for (float i = 1.0; i <= numBlurPixelsPerSide; i++) { 
    avgValue += texture(Texture0, v_texCoord.xy - i * newBlurSize * 
                          blurMultiplyVec) * incrementalGaussian.x;         
    avgValue += texture(Texture0, v_texCoord.xy + i * newBlurSize * 
                          blurMultiplyVec) * incrementalGaussian.x;         
    coefficientSum += 2.0 * incrementalGaussian.x;
    incrementalGaussian.xy *= incrementalGaussian.yz;
  }

  FragColor0 = avgValue / coefficientSum;
}
