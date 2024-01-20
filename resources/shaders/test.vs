#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float displ;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float scale;
out float o_displ;

void main()
{
	vec3 pos = vec3(aPos.x, aPos.y, displ);
    gl_Position = projection * view * model * (vec4(pos * scale, 1.0));
	o_displ = displ;
}