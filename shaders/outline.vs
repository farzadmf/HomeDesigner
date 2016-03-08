#version 400 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool modelOverride;

void main()
{
    if (!modelOverride)
        gl_Position = projection * view * model * vec4(position, 1.0f);
    else
        gl_Position = projection * view * vec4(position, 1.0f);
}