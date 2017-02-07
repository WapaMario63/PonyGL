#version 330 core
#define NR_POINT_LIGHTS 4

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;

in VS_OUT
{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
    //mat3 TBN;
    //vec4 FragPosLightSpace;
} fs_in;

uniform vec3 viewPos;
uniform bool gamma;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform sampler2D shadowMap;

/*float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}*/

vec3 CalcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0f);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));

    //float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir),0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float maxDistance = 1.5;
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (gamma ? distance * distance : distance));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    //float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = dot(-lightDir, light.direction);
    intensity = smoothstep(0.0,1.0, (intensity - light.outerCutOff)/(epsilon));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    diffuse *= intensity;
    specular *= intensity;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

    return (ambient + diffuse + specular);
}

void main(void)
{
    // Properties
    vec3 norm = normalize(fs_in.Normal);
    //vec3 norm = texture(material.normal, fs_in.TexCoords).rgb;

    //norm = normalize(norm * 2.0 - 1.0);
    //norm = normalize(fs_in.TBN * norm);

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    // Sunlight
    vec3 result = CalcDirectionalLight(dirLight, norm, viewDir);

    // Point Lights
    for (int i=0; i<NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], norm, fs_in.FragPos, viewDir);
    }


    //if (gamma)
    //    color = pow(color, vec3(1.0/2.2));
    //else
        color = vec4(result, 1.0);

    //vec4(1.0f,0.5f,0.2f,1.0f)
}
