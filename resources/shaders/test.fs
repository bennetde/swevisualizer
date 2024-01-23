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
void main()
{
	vec4 minCol = vec4(0.0, 0.0,0.0,1.0);
	vec4 maxCol = vec4(0.0, 0.0,1.0,1.0);


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
	FragColor = mycol;
	if(o_bathymetry > 0.0) {
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
} 