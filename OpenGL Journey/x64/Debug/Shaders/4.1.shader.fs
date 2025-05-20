#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

//oggetto che prende la texture 2d samplata
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float textureMixAlpha;

void main()
{
    //la texture samplata se mette come colore
    //texture(sampler2D texture, vec2 coordinates)
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), textureMixAlpha);
}