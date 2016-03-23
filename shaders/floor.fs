#version 400 core

out vec4 color;

uniform vec3 floorColor;
uniform bool textureMode;
uniform sampler2D floorTexture;

in vec2 TextureCoordinates;

void main()
{
    if (textureMode)
        // We should use the texture here!
        //color = vec4(0.2f, 0.2f, 0.2f, 1.0f);
        color = texture(floorTexture, TextureCoordinates);
    else
        color = vec4(floorColor, 1.0f);
}
