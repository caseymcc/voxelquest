#version 330

// result fbo
uniform sampler2D Texture0;

// radiosity buffer
uniform sampler2D Texture1;

// combineFBOWithWater
uniform sampler2D Texture2;


uniform bool testOn;



$
layout(location=0) in vec4 vertexPos;
layout(location=1) in vec4 vertexTex;

out vec2 TexCoord0;

void main() {

	TexCoord0 =vertexTex.xy;
	gl_Position =vertexPos;
}

$

in vec2 TexCoord0;

layout(location=0) out vec4 FragColor0;

void main() {

	vec4 oneVec = vec4(1.0);

	vec4 tex0 = texture(Texture0, TexCoord0.xy);
	vec4 tex1 = texture(Texture1, TexCoord0.xy);
	vec4 tex2 = texture(Texture2, TexCoord0.xy);
	//vec4 tex5 = texture(Texture5, TexCoord0.xy);
	
	// vec4 matValsGeom = tex5;
	// bool valIsGeom = (dot(matValsGeom.rgb,oneVec.rgb) != 0.0);

	vec3 newRad = tex1.rgb;//pow(tex1.rgb, vec3(2.0) );

	vec3 finalCol = 
		
		//mix(tex0.rgb, tex0.rgb + newRad*0.5, newRad); //tex0.rgb + newRad*0.25;//
		tex0.rgb + tex1.rgb*0.05;//mix(tex0.rgb, tex1.rgb,0.25);
	
	// vec3 resColGS = vec3( clamp(dot(finalCol, oneVec.xyz) / 3.0, 0.0, 1.0) );
	// finalCol = mix(
	// 	resColGS,
	// 	finalCol,
	// 	clamp(1.0 - distance(resColGS, finalCol), 0.0, 1.0) * 1.6
	// );
	
	
	
	
	if (testOn) {
		finalCol = tex1.rgb;
	}
	else {
		//finalCol = mix(finalCol,tex1.rgb,0.5); //finalCol = //tex0.rgb*0.25 + tex1.rgb;
	}
	
	//finalCol = tex1.rgb;
	
	// if (valIsGeom) {
	// 	finalCol.rgb = tex0.rgb;
	// }
	
	
	
	
    FragColor0 = vec4(finalCol.rgb, 1.0);//finalCol




}
