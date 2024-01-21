#version 330 core
out vec4 FragColor;



in float o_displ;

uniform float maxHeight;

void main()
{
	float normalizedHeight = (o_displ / maxHeight);
    FragColor = mix(vec4(0.0, 0.0, 0.5, 1.0), vec4(0.5, 0.5, 1.0, 1.0), normalizedHeight);	
} 