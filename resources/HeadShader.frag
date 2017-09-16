#version 330 core
out vec4 FragColor;


in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 surfaceColor;
uniform vec3 materialSpecular;
uniform float materialShininess;

uniform vec3 lightPosition;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

uniform float lightConstant;
uniform float lightLinear;
uniform float lightQuadratic;


void main()
{
    // ambient
    vec3 ambient = lightAmbient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * surfaceColor;
    
    // specular
    vec3 viewPos = vec3(0.0f, 0.0f, 0.0f);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = lightSpecular * (spec * materialSpecular); 

    //attenuation
    float distance = length(lightPosition - FragPos);
    float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * distance * distance);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation; 
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 