#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TextureCoordinates;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TextureCoordinates = vec2(textureCoordinates.x, 1.0f - textureCoordinates.y);
}