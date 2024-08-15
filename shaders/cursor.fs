#version 460 core

out vec4 FragColor;
in vec2 TexCoords;
in vec3 ourPosition;

uniform sampler2D texture0;

void main()
{
   vec4 color = texture(texture0, TexCoords);
   FragColor = color;
}