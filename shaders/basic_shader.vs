#version core 450

attribute vec3 position;
void main()
{
     gl_Position =vec4(position,1.0)+vec4(1.0);
}