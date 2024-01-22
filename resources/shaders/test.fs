#version 330 core
out vec4 FragColor;

// uniform float minScale;
in float o_displ; // ist zwischen (-5000, 5000)
in float o_minScale; // max Scale z.b. haben wir dann werte (-5000, 5000)
void main()
{
	vec4 minCol = vec4(1.0,0,0,1.0);
	vec4 maxCol = vec4(0.0,0.0,1.0,1.0); // interpolieren...

	//smoothstep (http://www.fundza.com/rman_shaders/smoothstep/)
	// float new_val01 = smoothstep(minScale, maxScale, o_displ);
	//mix (https://registry.khronos.org/OpenGL-Refpages/gl4/html/mix.xhtml)
	// vec4 mycol = mix(minCol, maxCol, new_val01);
	vec4 col = vec4(0.0f);
	// float val = step(10.01, o_displ);
	// r,g,b,a
	col = vec4(o_displ / 100.0f);
	FragColor = col;
} 