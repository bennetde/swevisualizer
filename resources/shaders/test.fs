#version 330 core
out vec4 FragColor;

in float o_displ;
in float o_bathymetry;
in float o_hu;
in float o_hv;

uniform float minHeight;
uniform float maxHeight;
uniform float minHu;
uniform float maxHu;
uniform float minHv;
uniform float maxHv;
uniform float minBathymetry;
uniform float maxBathymetry;
uniform bool bool_hu;
uniform bool bool_hv;
uniform vec4 minCol;
uniform vec4 maxCol;
uniform vec4 minhuCol;
uniform vec4 maxhuCol;
uniform vec4 minhvCol;
uniform vec4 maxhvCol;
uniform vec4 minBathymetryCol;
uniform vec4 maxBathymetryCol;

void main() {
	// smoothstep (http://www.fundza.com/rman_shaders/smoothstep/)
	float new_val01 = smoothstep(minHeight, maxHeight, o_displ);

	//mix (https://registry.khronos.org/OpenGL-Refpages/gl4/html/mix.xhtml)
	vec4 mycol = mix(minCol, maxCol, new_val01);
	
	

	//Show hu and hv at the same time
	if (bool_hu && bool_hv) {
		float speed = length(vec2(o_hu, o_hv));
		float huAlpha = smoothstep(minHu, maxHu, abs(speed));
		vec4 huLayer = mix(minhuCol, maxhuCol, huAlpha);
		FragColor = mix(mycol, huLayer, huAlpha);
	}
	// Only show hv
	else if (bool_hv) {
		float hvAlpha = smoothstep(minHv, maxHv, abs(o_hv));
		vec4 hvLayer = mix(minhvCol, maxhvCol, hvAlpha);
		FragColor = mix(mycol, hvLayer, hvAlpha);
	}
	// Only show hu
	else {
		float huAlpha = smoothstep(minHu, maxHu, abs(o_hu));
		vec4 huLayer = mix(minhuCol, maxhuCol, huAlpha);
		FragColor = mix(mycol, huLayer, huAlpha);
	}	

	if(o_bathymetry > 0.0) {
		float bathymetryAlpha = smoothstep(minBathymetry, maxBathymetry, o_bathymetry);
        FragColor = mix(minBathymetryCol, maxBathymetryCol, bathymetryAlpha);
	}
} 
