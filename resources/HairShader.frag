#version 330 core

in vec3 fragVert;
in vec3 fragVertPre;
in vec2 texCoord;

uniform vec3 lightPosition;
uniform vec4 surfColor;

uniform sampler2D texture_diffuse;

out vec4 FragColor;


void main()
{
	vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
	//vec4 surfaceColor = vec4(1.0, 0.84, 0.0, 1.0);
	vec4 surfaceColor = surfColor;
	//vec4 surfaceColor = texture(texture_diffuse, texCoord);
	vec4 ambient = vec4(0.4, 0.4, 0.4, 1.0);

	vec3 segmentDir = normalize(fragVert - fragVertPre);
	vec3 lightDir = normalize(lightPosition - fragVert);

	float diffCoef = 1.0 - dot(segmentDir, lightDir);
	//float diffCoef = mix(0.45, 1.0, dot(segmentDir, lightDir));

	vec4 diffuse = lightColor * diffCoef * surfaceColor;

    FragColor = min(diffuse, vec4(1.0));
}