#version 430
in vec2 TexCoord;
uniform sampler2D Texture;
out vec4 fragColor;
void main()
{
    fragColor = texture(Texture,TexCoord);
}