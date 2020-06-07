#version 430 
out vec4 FragColor;

uniform vec3 ObjectColor;
uniform vec3 lightColor;
void main()
{
    FragColor = vec4(lightColor * ObjectColor,1.0f);
}