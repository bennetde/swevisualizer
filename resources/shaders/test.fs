#version 330 core
out vec4 FragColor;

in float o_displ; // ist zwischen (-5000, 5000)
in float o_bathymetry;
in float o_hu;
in float o_hv;
// in float o_minHeight; // max Height z.b. haben wir dann werte (-5000, 5000)
// in float o_maxHeight;

uniform float minHeight; // add others...
uniform float maxHeight;
uniform float minHu;
uniform float maxHu;
uniform float minHv;
uniform float maxHv;
uniform float minBathymetry;
uniform float maxBathymetry;
uniform bool hu;
uniform bool hv;
uniform bool h;
void main()
{
	vec4 minCol = vec4(0.0, 0.0,0.0,0.5);
	vec4 maxCol = vec4(0.0, 0.0,1.0,0.5);

	vec4 minhuCol = vec4(0.0, 0.0,0.0,0.1);
	vec4 maxhuCol = vec4(0.0, 0.0,1.0,0.1);

	
	//muss noch überarbeitet werden, eventuell werden minHeight und maxHeight nicht neu gesetzt
	// smoothstep (http://www.fundza.com/rman_shaders/smoothstep/)
	float new_val01 = smoothstep(minHeight, maxHeight, o_displ);

	// float t = (o_displ - o_minHeight) / (o_maxHeight - o_minHeight);
	// float new_val01 = clamp(t, 0.0, 1.0);

	//mix (https://registry.khronos.org/OpenGL-Refpages/gl4/html/mix.xhtml)
	vec4 mycol = mix(minCol, maxCol, new_val01);
	// mycol.r = o_hu;
	// mycol.g = o_hv;
	// vec4 col = vec4(0.0f);
	// float val = step(10.01, o_displ);
	// r,g,b,a
	// col = vec4(o_displ / 100.0f);
	//FragColor = mycol;
	
	float huAlpha = smoothstep(minHu, maxHu, o_hu);
	vec4 huLayer = mix(minhuCol, maxhuCol, huAlpha);
	FragColor = mix(mycol, huLayer, huAlpha);
   
	vec4 minBathymetryCol = vec4(0.0, 0.5, 0.0, 1.0);
	vec4 maxBathymetryCol = vec4(0.4, 0.2, 0.0, 1.0);
	if(o_bathymetry > 0.0) {
		float bathymetryAlpha = smoothstep(minBathymetry, maxBathymetry, o_bathymetry);
        FragColor = mix(minBathymetryCol, maxBathymetryCol, bathymetryAlpha);
	}
} 
