#version 330 core
out vec4 FragColor;

// uniform float minScale;
in float o_displ; // ist zwischen (-5000, 5000)
in float o_minHeight; // max Height z.b. haben wir dann werte (-5000, 5000)
in float o_maxHeight;
void main()
{
	vec4 minCol = vec4(1.0,1.0,1.0,1.0);
	vec4 maxCol = vec4(1.0,0.0,0.0,1.0);


	//muss noch überarbeitet werden, eventuell werden minHeight und maxHeight nicht neu gesetzt
	// smoothstep (http://www.fundza.com/rman_shaders/smoothstep/)
	float new_val01 = smoothstep(o_minHeight, o_maxHeight, o_displ);

	// float t = (o_displ - o_minHeight) / (o_maxHeight - o_minHeight);
	// float new_val01 = clamp(t, 0.0, 1.0);

	//mix (https://registry.khronos.org/OpenGL-Refpages/gl4/html/mix.xhtml)
	vec4 mycol = mix(minCol, maxCol, new_val01);
	// vec4 col = vec4(0.0f);
	// float val = step(10.01, o_displ);
	// r,g,b,a
	// col = vec4(o_displ / 100.0f);
	FragColor = mycol;
} 