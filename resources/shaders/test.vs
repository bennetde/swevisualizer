#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float displ;
layout (location = 2) in float hu;
layout (location = 3) in float hv;
layout (location = 4) in float bathymetry;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float scale;
uniform float minHeight; // add others...
uniform float maxHeight;
out float o_displ;
out float o_minHeight;
out float o_maxHeight;

void main()
{
	vec3 pos = vec3(-aPos.x, aPos.y, 0.0);
    gl_Position = projection * view * model * (vec4(pos * scale, 1.0));
	o_displ = displ;
	o_minHeight = minHeight;
	o_maxHeight = maxHeight;
}