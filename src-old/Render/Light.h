#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Render/Shader.h>
#include <Camera.h>

class Light
{
public:
    enum LightType
    {
        SPOTLIGHT,
        POINTLIGHT,
        DIRECTIONAL
    };
    enum ModelType
    {
        CUBE,
        CUSTOM
    };

    Light(Camera cam);
    void Draw();

    // SET
    void setPosition(glm::vec3 pos);
    void setDirection(glm::vec3 dir);
    void setLight(LightType light);
    void setColor(glm::vec3 Color);
    void setDirectionalLightShader(Shader shader);
    void setPointLightShader(Shader shader);
    void setSpotLightShader(Shader shader);
    // GET
    glm::vec3 getPosition();
    glm::vec3 getDirection();
    LightType getLightType();
    Shader getDirectionalLightShader();
    Shader getPointLightShader();
    Shader getSpotLightShader();
private:
    GLuint VAO, VBO;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    LightType type;
    Shader directionLightShader;
    Shader pointLightShader;
    Shader spotLightShader;

    void SetupBasicLight(glm::vec3 color, Shader shader, Camera cam);
    void SetupDirectionalLight(glm::vec3 Direction, Camera cam, const Shader shader);
    void SetupPointLight(glm::vec3 Position, Camera cam, const Shader shader);
    void SetupSpotLight(glm::vec3 position, glm::vec3 direction, Camera cam, const Shader shader);

    void SetupVAOandVBO();
};

#endif // LIGHT_H
