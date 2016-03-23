#version 400 core

out vec4 color;

uniform vec3 floorColor;
uniform bool textureMode;

void main()
{
    if (textureMode)
        // We should use the texture here!
        color = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    else
        color = vec4(floorColor, 1.0f);
}
