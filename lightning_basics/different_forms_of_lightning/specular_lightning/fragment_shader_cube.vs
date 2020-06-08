#version 430
out vec4 FragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{	
    float AS = 0.5;
    vec3 ambient = AS * lightColor;
    FragColor = vec4(ambient * objectColor,1.0);
}
