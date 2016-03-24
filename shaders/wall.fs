#version 400 core

out vec4 color;

uniform vec3 wallColor;
uniform bool textureMode;
uniform sampler2D wallTexture;

in vec2 TextureCoordinates;

void main()
{
    if (textureMode)
        color = texture(wallTexture, TextureCoordinates);
    else
        color = vec4(wallColor, 1.0f);
}
