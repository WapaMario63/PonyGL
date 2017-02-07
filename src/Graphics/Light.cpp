#include "Light.h"
#include <iostream>

Light::Light(GLSLShader shader)
{
    _shader = shader;

    _baseSun = LightType::LtSun();
    _basePoint = LightType::LtPoint();
    _baseSpot = LightType::LtSpot();
}

void Light::createSunLight(glm::vec3 direction, glm::vec3 color, float intensity)
{
    _baseSun.base.color = color;
    _baseSun.base.intensity = intensity;
    _base.color = color;
    _base.intensity = intensity;
    _baseSun.direction = direction;
    _type = LightType::LT_SUN;
}

void Light::Draw(Camera cam)
{
    // Draw a light cube if it is not a sun
    if (_type != LightType::LT_SUN)
    {
        _entityShader.load();
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 model;
        view = cam.GetViewMatrix();
        projection = glm::perspective(cam.zoom, (GLfloat)DEFAULT_WINDOW_RESOLUTION_WIDTH / (GLfloat)DEFAULT_WINDOW_RESOLUTION_HEIGHT, 0.1f, 100.f);

        glUniformMatrix4fv(glGetUniformLocation(_entityShader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(_entityShader.GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::translate(model, _basePoint.position);
        model = glm::scale(model, glm::vec3(0.2f,0.2f,0.2f));
        glUniformMatrix4fv(glGetUniformLocation(_entityShader.GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    GetUniforms(_shader);
}

void Light::GetUniforms(GLSLShader shader)
{
    _baseSun.base.ambient = _base.ambient;
    _baseSun.base.diffuse = _base.diffuse;
    _baseSun.base.specular = _base.specular;

    // Core Materials
    glUniform1i(glGetUniformLocation(shader.GetProgram(), "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(shader.GetProgram(), "material.specular"), 1);
    glUniform1f(glGetUniformLocation(shader.GetProgram(), "material.shininess"), _base.intensity);

    // Directinal/Sun
    glUniform3f(glGetUniformLocation(shader.GetProgram(), "dirLight.direction"),
                _baseSun.direction.x, _baseSun.direction.y, _baseSun.direction.z);
    glUniform3f(glGetUniformLocation(shader.GetProgram(), "dirLight.ambient"),
                _baseSun.base.ambient.x, _baseSun.base.ambient.y, _baseSun.base.ambient.z);
    glUniform3f(glGetUniformLocation(shader.GetProgram(), "dirLight.diffuse"),
                _baseSun.base.diffuse.x, _baseSun.base.diffuse.y, _baseSun.base.diffuse.z);
    glUniform3f(glGetUniformLocation(shader.GetProgram(), "dirLight.specular"),
                _baseSun.base.specular.x, _baseSun.base.specular.y, _baseSun.base.specular.z);

    // Point Light
    glUniform3f(glGetUniformLocation(shader.GetProgram(), "pointLights.position"),
                _basePoint.position.x, _basePoint.position.y, _basePoint.position.z);
    glUniform3f(glGetUniformLocation(shader.GetProgram(), "pointLights.ambient"),
                _basePoint.base.ambient.x, _basePoint.base.ambient.y, _basePoint.base.ambient.z);
    glUniform3f(glGetUniformLocation(shader.GetProgram(), "pointLights.diffuse"),
                _basePoint.base.diffuse.x, _basePoint.base.diffuse.y, _basePoint.base.diffuse.z);
    glUniform3f(glGetUniformLocation(shader.GetProgram(), "pointLights.specular"),
                _basePoint.base.specular.x, _basePoint.base.specular.y, _basePoint.base.specular.z);
    glUniform1f(glGetUniformLocation(shader.GetProgram(), "pointLights.constant"), _basePoint.constant);
    glUniform1f(glGetUniformLocation(shader.GetProgram(), "pointLights.linear"), _basePoint.linear);
    glUniform1f(glGetUniformLocation(shader.GetProgram(), "pointLights.quadratic"), _basePoint.quadratic);

}

void Light::Setup()
{
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

}

