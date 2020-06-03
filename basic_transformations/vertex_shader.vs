#version 430
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 transform;
void main()
{
    gl_Position = transform * vec4(Pos,1.0);
    TexCoord = aTexCoord;
}