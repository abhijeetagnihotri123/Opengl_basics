#version 430
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 viewPos;
uniform vec3 ambient_cube;
uniform vec3 diffuse_cube;
uniform vec3 specular_cube;
uniform float shininess;
uniform vec3 position_light;
uniform vec3 ambient_light;
uniform vec3 diffuse_light;
uniform vec3 specular_light;

void main()
{	
    vec3 ambient = ambient_light * ambient_cube;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(position_light - FragPos);
    float Diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diffuse_light * (Diff * diffuse_cube);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),shininess);
    vec3 SPC = specular_light * (spec * specular_cube);
    //vec3 result = ambient + diffuse + SPC;
    FragColor = vec4(ambient+diffuse+SPC,1.0); 
    //FragColor = vec4(diffuse_light+ambient_light,1.0f);
    // float AS = 0.5;
    // vec3 ambient = AS * lightColor;
    // vec3 result = ambient * objectColor
    // FragColor = vec4(result,1.0);
}
