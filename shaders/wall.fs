#version 400 core

out vec4 color;

uniform vec3 wallColor;
uniform bool textureMode;

void main()
{
    if (textureMode)
        // We should use the texture here!
        color = vec4(0.4f, 0.4f, 0.4f, 1.0f);
    else
        color = vec4(wallColor, 1.0f);
}
