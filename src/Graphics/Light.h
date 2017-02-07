#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include "pne_utils.h"
#include "GLSLShader.h"
#include "Camera.h"

struct LightType
{
    struct LtBase
    {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 color;
        float intensity;
    } lightBase;

    struct LtSun
    {
        LtBase base;
        glm::vec3 direction;
    } lightSun;

    struct LtPoint
    {
        LtBase base;
        glm::vec3 position;
        float constant;
        float linear;
        float quadratic;
    } lightPoint;

    struct LtSpot
    {
        LtBase base;
        glm::vec3 position;
        glm::vec3 direction;
        float cutOffStart;
        float cutOffEnd;
        float constant;
        float linear;
        float quadratic;
    } lightSpot;

    enum LtTypeList
    {
        LT_SUN = 0,
        LT_POINT = 1,
        LT_SPOT = 2,
    };
};

class Light
{
public:
    Light(GLSLShader shader);
    void createSunLight(glm::vec3 direction, glm::vec3 color, float intensity);
    void createPointLight(glm::vec3 position, glm::vec3 color, float intensity,
                          float constant = 1.0f, float linear = 0.09f,
                          float quadratic = 0.032f);
    void createSpotLight(glm::vec3 position, glm::vec3 direction, float cutOffStart,
                         float cutOffEnd, float constant = 1.0f, float linear = 0.09f,
                         float quadratic = 0.032f);
    void Draw(Camera cam);

    inline GLSLShader GetShader() const { return _shader; }
    inline LightType::LtTypeList GetType() const { return _type; }
    inline glm::vec3 GetPosition() const { return _position; }
    inline glm::vec3 GetDirection() const { return _direction; }
    inline glm::vec3 GetColor() const { return _base.color; }
    inline float GetIntensity() const { return _base.intensity; }

    inline void setShader(GLSLShader shader) { _shader = shader; }
    inline void setDirection(glm::vec3 direction) { _direction = direction; }
    inline void setPosition(glm::vec3 position) { _position = position; }
    inline void setColor(glm::vec3 color) { _base.color = color; }
    inline void setIntensity(float intensity) { _base.intensity = intensity; }
    inline void setEntityShader(GLSLShader shader) { _entityShader = shader; }
    inline void setMaterialProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        { _base.ambient = ambient; _base.diffuse = diffuse; _base.specular = specular;}

private:
    void GetUniforms(GLSLShader shader);
    void Setup();

    GLuint VAO, VBO;

    LightType::LtTypeList _type;
    LightType::LtBase _base;
    LightType::LtSun _baseSun;
    LightType::LtPoint _basePoint;
    LightType::LtSpot _baseSpot;
    GLSLShader _shader;
    GLSLShader _entityShader;

    glm::vec3 _position;
    glm::vec3 _direction;
    float _constant;
    float _linear;
    float _quadratic;

    float _cutOffStart;
    float _cutOffEnd;
};

struct MasterLights
{
    std::vector<Light> lights;
};

#endif // LIGHT_H
