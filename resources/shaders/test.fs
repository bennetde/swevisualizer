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
uniform vec4 userColor;
uniform bool useUserColor;
void main()
{	
	vec4 minCol = vec4(0.5, 0.7,0.8,1.0);
	vec4 maxCol = vec4(0.0, 0.0,1.0,1.0);
	//Farbe des Users wird verwendet
	vec4 baseColor = useUserColor ? userColor : minCol;
	

	vec4 minhuCol = vec4(0.2, 0.0,0.0,1.0);
	vec4 maxhuCol = vec4(0.5, 0.0,0.0,1.0);

	vec4 minhvCol = vec4(0.2, 0.0,0.0,1.0);
	vec4 maxhvCol = vec4(0.5, 0.0,0.0,1.0);

	
	//muss noch überarbeitet werden, eventuell werden minHeight und maxHeight nicht neu gesetzt
	// smoothstep (http://www.fundza.com/rman_shaders/smoothstep/)
	float new_val01 = smoothstep(minHeight, maxHeight, o_displ);

	// float t = (o_displ - o_minHeight) / (o_maxHeight - o_minHeight);
	// float new_val01 = clamp(t, 0.0, 1.0);

	//mix (https://registry.khronos.org/OpenGL-Refpages/gl4/html/mix.xhtml)
	//vec4 mycol = mix(minCol, maxCol, new_val01);
	vec4 mycol = mix(baseColor, maxCol, new_val01);
	// mycol.r = o_hu;
	// mycol.g = o_hv;
	// vec4 col = vec4(0.0f);
	// float val = step(10.01, o_displ);
	// r,g,b,a
	// col = vec4(o_displ / 100.0f);
	//FragColor = mycol;
	
	// if (hv) {
	// 	float hvAlpha = smoothstep(minHv, maxHv, abs(o_hv));
	// 	vec4 hvLayer = mix(minhvCol, maxhvCol, hvAlpha);
	// 	FragColor = mix(mycol, hvLayer, hvAlpha);
	// }
	// else {
		float huAlpha = smoothstep(minHu, maxHu, abs(o_hu));
		vec4 huLayer = mix(minhuCol, maxhuCol, huAlpha);
		FragColor = mix(mycol, huLayer, huAlpha);
		// FragColor = mix(mycol, huLayer, 1.0);
	// }
	   
	vec4 minBathymetryCol = vec4(0.0, 0.5, 0.0, 1.0);
	vec4 maxBathymetryCol = vec4(0.4, 0.2, 0.0, 1.0);
	if(o_bathymetry > 0.0) {
		float bathymetryAlpha = smoothstep(minBathymetry, maxBathymetry, o_bathymetry);
        FragColor = mix(minBathymetryCol, maxBathymetryCol, bathymetryAlpha);
	}

	// vec4 minBathymetryCol = vec4(0.0, 0.9, 0.0, 1.0);
	// vec4 maxBathymetryCol = vec4(0.0, 0.4, 0.0, 1.0);
	// float bathymetryAlpha = smoothstep(minBathymetry, maxBathymetry, o_bathymetry);
    // vec4 bathymetryLayer = mix(minBathymetryCol, maxBathymetryCol, bathymetryAlpha);
	// FragColor = mix(FragColor, bathymetryLayer, bathymetryAlpha);
	// if (o_bathymetry > 0) {
	// 	vec4 minBathymetryCol = vec4(0.0, 0.5, 0.0, 1.0);
	// 	vec4 maxBathymetryCol = vec4(0.4, 0.2, 0.0, 1.0);
	// 	float bathymetryAlpha = smoothstep(0, maxBathymetry, o_bathymetry);
    // 	vec4 bathymetryLayer = mix(minBathymetryCol, maxBathymetryCol, 1.0);
	// 	FragColor = bathymetryLayer;
	// }
} 
