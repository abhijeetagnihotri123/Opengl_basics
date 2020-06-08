#version 430
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(Pos,1.0);
    Normal = aNormal;
    FragPos = vec3(model*vec4(Pos,1.0));
}
