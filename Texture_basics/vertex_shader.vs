#version 430
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 aColor;
uniform float xOffset;
out vec3 ourColor;
void main()
{
    gl_Position = vec4(pos.x+xOffset,-pos.y,pos.z,1.0);
    ourColor = aColor;
}