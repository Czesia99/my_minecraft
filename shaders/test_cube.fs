#version 460 core

out vec4 FragColor;

in vec3 ourPosition;
in vec2 TexCoords;
uniform vec3 isValid;


// uniform float textureUnit;


void main()
{
    // vec3 NewTexCoord = vec3(TexCoords.x, TexCoords.y, BlockType);
    FragColor = vec4(isValid, 0.5);
}