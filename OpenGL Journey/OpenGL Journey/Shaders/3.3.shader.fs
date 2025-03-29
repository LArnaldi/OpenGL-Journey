#version 330 core
out vec4 FragColor;

in vec3 ourColor;
uniform float alphaColor;
in vec3 positionToColor;
void main()
{
    FragColor = vec4(positionToColor, alphaColor);
}