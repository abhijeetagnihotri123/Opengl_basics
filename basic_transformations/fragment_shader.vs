#version 430
in vec2 TexCoord;
uniform sampler2D ourTexture;
void main()
{
    gl_FragColor = texture(ourTexture,TexCoord);
}