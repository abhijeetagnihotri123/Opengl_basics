#version 430
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;
uniform vec3 viewPos;
uniform sampler2D diffuse;
uniform sampler2D specular;
uniform float shininess;

uniform vec3 position;
uniform vec3 ambient;
uniform vec3 Diffuse;
uniform vec3 SPECULAR;

void main()
{   
    vec3 Ambient = ambient * texture(diffuse,TexCoord).rgb;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(position - FragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 DIFF = Diffuse * diff * texture(diffuse,TexCoord).rgb;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewPos,reflectDir),0),shininess);
    vec3 SPC = SPECULAR * spec * texture(specular,TexCoord).rgb;
    FragColor = vec4((Ambient + DIFF + SPC),1.0);
    // FragColor = vec4(texture(specular,TexCoord).rgb,1.0);
}