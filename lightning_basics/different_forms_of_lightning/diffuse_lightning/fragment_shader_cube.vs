#version 430
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{	
    float AS = 0.5;
    vec3 ambient = AS * lightColor;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos-FragPos);
    float diff = max(dot(norm,lightDir),0);
    vec3 diffuse = diff * lightColor;
    vec3 result = (ambient+diffuse)*objectColor;
    FragColor = vec4(result,1.0);
}
