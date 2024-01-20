#version 330 core
out vec4 FragColor;


in float o_displ;
void main()
{
	vec4 col = vec4(0.0f);
	// float val = step(10.01, o_displ);
	col = vec4(o_displ / 15.0f);
	FragColor = col;
} 