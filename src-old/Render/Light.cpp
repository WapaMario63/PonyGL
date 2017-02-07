#include "Light.h"

GLfloat vertices[] =
{
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

void Light::SetupVAOandVBO()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

glm::vec3 Light::getPosition() { return this->position; }
glm::vec3 Light::getDirection() { return this->direction; }
Light::LightType Light::getLightType() { return this->type; }
Shader Light::getDirectionalLightShader() { return this->directionLightShader; }
Shader Light::getPointLightShader() { return this->pointLightShader; }
Shader Light::getSpotLightShader() { return this->spotLightShader; }

void Light::setPosition(glm::vec3 pos) { this->position = pos; }
void Light::setDirection(glm::vec3 dir) { this->direction = dir; }
void Light::setLight(LightType light) { this->type = light; }
void Light::setColor(glm::vec3 Color) { this->color = Color; }
void Light::setDirectionalLightShader(Shader shader) { this->directionLightShader = shader; }
void Light::setPointLightShader(Shader shader) { this->pointLightShader = shader; }
void Light::setSpotLightShader(Shader shader) { this->spotLightShader = shader; }

Light::Light(Camera cam)
    : position(glm::vec3(0.0f,0.0f,0.0f)),
      direction(glm::vec3(0.0f,0.0f,0.0f)),
      color(glm::vec3(1.0f,1.0f,1.0f)),
      directionLightShader(Shader("res/shader/Light_DirectionalV.pvs", "res/shader/Light_DirectionalF.pfs")),
      pointLightShader(Shader("res/shader/Light_PointV.pvs", "res/shader/Light_PointF.pfs")),
      spotLightShader(Shader("res/shader/Light_SpotV.pvs", "res/shader/Light_SpotF.pfs"))
{
    if (this->type == DIRECTIONAL)
        SetupDirectionalLight(this->direction, cam, this->directionLightShader);
    else if (this->type == POINTLIGHT)
        SetupPointLight(this->position, cam, this->pointLightShader);
    else if (this->type == SPOTLIGHT)
        SetupSpotLight(this->position, this->direction, cam, this->spotLightShader);
    else
        SetupBasicLight(glm::vec3(0.9f, 1.0f, 0.9f),Shader("res/shader/ModelVertexShader.pvs", "res/shader/ModelFragmentShader.pfs"), cam);

    /*switch (this->type)
    {
    case DIRECTIONAL:
        SetupDirectionalLight(this->direction, cam, this->directionLightShader);
    case POINTLIGHT:
        SetupPointLight(this->position, cam, this->pointLightShader);
    case SPOTLIGHT:
        SetupSpotLight(this->position, this->direction, cam, this->spotLightShader);
    default:
        SetupBasicLight(glm::vec3(0.9f, 1.0f, 0.9f),Shader("res/shader/ModelVertexShader.pvs", "res/shader/ModelFragmentShader.pfs"), cam);
    }*/
}

void Light::SetupBasicLight(glm::vec3 color, Shader shader, Camera cam)
{
    SetupVAOandVBO();

    GLint objectColorLoc = glGetUniformLocation(shader.Program, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(shader.Program, "lightColor");
    GLint lightPosLoc = glGetUniformLocation(shader.Program, "lightPos");
    GLint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
    glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);
    glUniform3f(lightColorLoc, color.x, color.y, color.z);
    glUniform3f(lightPosLoc, position.x, position.y, position.z);
    glUniform3f(viewPosLoc, cam.position.x, cam.position.y, cam.position.z);
}

void Light::SetupDirectionalLight(glm::vec3 Direction, Camera cam, const Shader shader)
{
    SetupVAOandVBO();

    this->position = glm::vec3();

    GLint lightDirLoc = glGetUniformLocation(shader.Program, "light.ambient");
    GLint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
    glUniform3f(lightDirLoc, Direction.x, Direction.y, Direction.z);
    glUniform3f(viewPosLoc, cam.position.x, cam.position.y, cam.position.z);

    GLint ambientLoc = glGetUniformLocation(shader.Program, "light.ambient");
    GLint diffuseLoc = glGetUniformLocation(shader.Program, "light.diffuse");
    GLint speculiarLoc = glGetUniformLocation(shader.Program, "light.speculiar");
    glUniform3f(ambientLoc, 0.2f, 0.5f, 0.2f);
    glUniform3f(diffuseLoc, 0.5f, 0.5f, 0.5f);
    glUniform3f(speculiarLoc, 1.0f, 1.0f, 1.0f);

    glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);
}

void Light::SetupPointLight(glm::vec3 Position, Camera cam, const Shader shader)
{
    SetupVAOandVBO();

    GLint lightPosLoc = glGetUniformLocation(shader.Program, "light.position");
    GLint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
    glUniform3f(lightPosLoc, Position.x, Position.y, Position.z);
    glUniform3f(viewPosLoc, cam.position.x, cam.position.y, cam.position.z);

    GLint ambientLoc = glGetUniformLocation(shader.Program, "light.ambient");
    GLint diffuseLoc = glGetUniformLocation(shader.Program, "light.diffuse");
    GLint speculiarLoc = glGetUniformLocation(shader.Program, "light.speculiar");
    glUniform3f(ambientLoc, 0.2f, 0.5f, 0.2f);
    glUniform3f(diffuseLoc, 0.5f, 0.5f, 0.5f);
    glUniform3f(speculiarLoc, 1.0f, 1.0f, 1.0f);

    glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);

    glUniform1f(glGetUniformLocation(shader.Program, "light.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(shader.Program, "light.linear"), 0.09f);
    glUniform1f(glGetUniformLocation(shader.Program, "light.constant"), 0.032f);
}

void Light::SetupSpotLight(glm::vec3 position, glm::vec3 direction, Camera cam, const Shader shader)
{
    SetupVAOandVBO();

    GLint lightPosLoc = glGetUniformLocation(shader.Program, "light.position");
    GLint lightSpotDirLoc = glGetUniformLocation(shader.Program, "light.direction");
    GLint lightSpotCutOffLoc = glGetUniformLocation(shader.Program, "light.cutOff");
    GLint lightSpotOuterCutOffLoc = glGetUniformLocation(shader.Program, "light.outerCutOff");
    GLint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
    glUniform3f(lightPosLoc, cam.position.x, cam.position.y, cam.position.z);
    glUniform3f(lightSpotDirLoc, cam.front.x, cam.front.y, cam.front.z);
    glUniform1f(lightSpotCutOffLoc, glm::cos(glm::radians(12.5f)));
    glUniform1f(lightSpotOuterCutOffLoc, glm::cos(glm::radians(17.5f)));
    glUniform3f(viewPosLoc, cam.position.x, cam.position.y, cam.position.z);

    glUniform3f(glGetUniformLocation(shader.Program, "light.ambient"), 0.1f, 0.1f, 0.1f);
    glUniform3f(glGetUniformLocation(shader.Program, "light.diffuse"), 0.8f, 0.8f, 0.8f);
    glUniform3f(glGetUniformLocation(shader.Program, "light.speculiar"), 1.0f, 1.0f, 1.0f);

    glUniform1f(glGetUniformLocation(shader.Program, "light.constant"),  1.0f);
    glUniform1f(glGetUniformLocation(shader.Program, "light.linear"),    0.09);
    glUniform1f(glGetUniformLocation(shader.Program, "light.quadratic"), 0.032);

    glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);
}

void Light::Draw()
{   
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
