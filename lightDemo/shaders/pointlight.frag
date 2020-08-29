#version 330

uniform sampler2D texmap;
uniform sampler2D texmap1;
uniform sampler2D texmap2;
uniform sampler2D texmap3;
uniform sampler2D texmap4;
uniform sampler2D texnormal;
uniform bool enableFog;

uniform int texMode;
uniform bool shadowMode;
out vec4 colorOut;

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};
uniform Materials mat;


in Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
	vec2 tex_coord;
	vec4 pos;
} DataIn;

struct LightProperties {
    int isEnabled; // true to apply this light in this invocation
    int isLocal;	// true for a point light or a spotlight,
                    // false for a positional light
    int isSpot;	// true if the light is a spotlight
    vec3 color;		// color of light
    vec3 position;	// location of light, if is Local is true,
						// otherwise the direction toward the light
    vec3 coneDirection; // spotlight attributes
    float spotCosCutoff;
									// other properties you may desire
};


uniform LightProperties Lights[8];

void main() {

	vec4 spec = vec4(0.0);
	vec4 texel, texel1; 
	vec3 n;
    if(texMode == 3)
        n = normalize(2.0 * texture2D(texnormal, DataIn.tex_coord).rgb - 1.0);
	else
        n = normalize(DataIn.normal);
	vec3 l;
	vec3 e = normalize(DataIn.eye);
	vec3 sd ;//= normalize(vec3(-spotDir));
	float intensity = 0.0f;
	float totIntensity = 0.0f;
	float intSpec = 0.0f;
	if(shadowMode)  //constant color
		colorOut = vec4(0.5, 0.5, 0.5, 1.0);
    else{
    for(int i = 0; i<8; i++){
        if(Lights[i].isEnabled == 1)  {
            if(Lights[i].isLocal == 1){
                if(Lights[i].isSpot == 1)  {     //Scene iluminated by a spotlight
                    l = normalize(Lights[i].position-DataIn.pos.xyz);
                    sd = normalize(-Lights[i].coneDirection);
                    if(dot(l, sd) > Lights[i].spotCosCutoff)  {				//inside cone?
                        intensity = max(dot(n,l), 0.0);
                        totIntensity += intensity;
                        if (intensity > 0.0) {
                            vec3 h = normalize(l + e);
                            intSpec = max(dot(h,n), 0.0);
                            spec+= mat.specular * pow(intSpec, mat.shininess);
                        }
                    }
                }
                else {				//Scene iluminated by a pointlight
                    l = normalize(Lights[i].position-DataIn.pos.xyz);
                    intensity = max(dot(n,l), 0.0);
                    totIntensity += intensity;
                    if (intensity > 0.0) {
                        vec3 h = normalize(l + e);
                        intSpec = max(dot(h,n), 0.0);
                        spec += mat.specular * pow(intSpec, mat.shininess);
                    }
                }
            }
            else{
            l = normalize(Lights[i].position-DataIn.pos.xyz);
            intensity = max(dot(n,l), 0.0);
            totIntensity += intensity;
            if (intensity > 0.0) {
                vec3 h = normalize(l + e);
                intSpec = max(dot(h,n), 0.0);
                spec += mat.specular * pow(intSpec, mat.shininess);
                }  
            }
        }
	}


	if(texMode == 0)
	{
		texel = texture(texmap2, DataIn.tex_coord);  // texel from lighwood.tga
		colorOut = vec4(max(totIntensity * texel.rgb + spec.rgb, 0.1* texel.rgb), mat.diffuse.a);
	}
    else if (texMode == 1) 
	{
			colorOut = vec4(max(totIntensity * mat.diffuse.rgb + spec.rgb, mat.ambient.rgb), mat.diffuse.a);
	}
	else if(texMode == 2)  {	// modulated texture for particle
		texel = texture(texmap3, DataIn.tex_coord);
		texel.a = texel.r;     //this is a trick because the particle.bmp does not have alpha channel
		colorOut = mat.diffuse * texel;
	}
	else if (texMode == 3) // diffuse color is replaced by texel color, with specular area or ambient (0.1*texel)
	{
		texel = texture(texmap, DataIn.tex_coord);  // texel from lighwood.tga
		texel1 = texture(texmap1, DataIn.tex_coord);  // texel from checker.tga
		colorOut = vec4(max(totIntensity*texel.rgb*texel1.rgb + spec.rgb,0.1*texel.rgb*texel1.rgb), mat.diffuse.a);
	}
	else if(texMode == 4)
	{
		texel = texture(texmap4, DataIn.tex_coord); 
		if(texel.a == 0.0) discard;
		colorOut = vec4(max(totIntensity * texel.rgb + spec.rgb, 0.1* texel.rgb), mat.diffuse.a);
	}
	else if(texMode == 5)
	{
		texel = texture(texmap, DataIn.tex_coord);  //texel from element flare texture
		if((texel.a == 0.0)  || (mat.diffuse.a == 0.0) ) discard;
		else
			colorOut = mat.diffuse * texel;
	}
	
	// FOG	
	if (enableFog) {
		float dist = length(DataIn.pos);
		float fogFactor = 0;
		float FogDensity = 0.005f;
		vec3 fogColor = vec3(0.8, 0.6, 0.87);

		//fogFactor = (200 - dist)/(200 - 50); // linear
		dist = dist - 35; // Fog start distance
		fogFactor = 1.0 / exp(dist * FogDensity); // exponential

		fogFactor = clamp(fogFactor, 0.0,1.0);
		//colorOut = vec4(fogColor * (1-fogFactor) + colorOut.xyz * fogFactor, mat.transparency);

		if(texMode == 1){
		colorOut = vec4(fogColor * (1-fogFactor) + colorOut.xyz * fogFactor, mat.diffuse.a);;

		}else if (texMode == 0 || texMode == 3) {
			colorOut = vec4(fogColor * (1-fogFactor) + colorOut.xyz * fogFactor, texel.a);
		}
	}
}

//colorOut = max(intensity * mat.diffuse + spec, mat.ambient)* //texture(texmap, DataIn.tex_coord) * texture(texmap1, DataIn.tex_coord);

}
