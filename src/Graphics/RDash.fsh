#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform float shininess;
uniform vec3 viewPos;

void main()
{
    color = vec4(texture(texture_diffuse1, TexCoords));
}
