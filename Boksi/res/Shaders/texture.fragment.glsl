#version 430 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D screenTexture;

void main() 
{
    FragColor = texture(screenTexture, TexCoord);
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}