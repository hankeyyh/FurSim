#version 330 core

layout (location = 0) in vec3 pos;



uniform mat4 projMatrix;
uniform mat4 mvMatrix;

void main()
{
	gl_Position = projMatrix * mvMatrix * vec4(pos, 1);
}