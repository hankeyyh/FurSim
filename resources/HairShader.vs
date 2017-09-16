#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vPrePos;
layout (location = 2) in vec2 vTexCoord;

out vec3 fragVert;
out vec3 fragVertPre;
out vec2 texCoord;

uniform mat4 modelView;
uniform mat4 projection;

void main()
{
	fragVert = vec3(modelView * vec4(vPos, 1.0));
	fragVertPre = vec3(modelView * vec4(vPrePos, 1.0));
    gl_Position = projection * modelView * vec4(vPos, 1.0);

    texCoord = vTexCoord;
}