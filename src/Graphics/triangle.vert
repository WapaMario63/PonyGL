#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
//layout (location = 3) in vec3 tangent;
//layout (location = 4) in vec3 bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat4 lightSpaceMatrix;

out VS_OUT
{
    out vec3 Normal;
    out vec3 FragPos;
    out vec2 TexCoords;
    //out mat3 TBN;
    //out vec3 TangentViewPos;
    //out vec3 TangentLightPos;
    //out vec3 TangentFragPos;
    //out vec4 FragPosLightSpace;
} vs_out;

//uniform vec3 viewPos;

void main(void)
{
    /*vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(normal, 0.0)));

    mat3 TBN = transpose(mat3(T,B,N));
    vs_out.TangentLightPos = TBN * viewPos;
    vs_out.TangentViewPos = TBN * lightPos;
    vs_out.TangentFragPos = TBN * vec3(model * vec4(position, 0.0));*/

    gl_Position = projection * view * model * vec4(position, 1.0f);
    vs_out.FragPos = vec3(model * vec4(position, 1.0f));
    vs_out.Normal = mat3(transpose(inverse(model))) * normal;
    vs_out.TexCoords = texCoords;
    //vs_out.TBN = mat3(T, B, N);
    //vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
}
