#version 400 core

in vec2 TextureCoordinates;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{
    //color = vec4(0.6f, 0.6f, 0.6f, 1.0f);
    color = texture(texture_diffuse1, TextureCoordinates);
}
