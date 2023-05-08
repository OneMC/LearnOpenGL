//
//  blend.cpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/4/27.
//

#include "blend.hpp"
#include <map>

extern void loadTexture(char const * texturePath, unsigned int *texture, int colorType);

void BlendRender::setupTransparentVAO() {
//    float transparentVertices[] = {
//        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
//        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
//        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
//        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
//
//        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
//        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
//        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
//    };
    
    float transparentVertices[] = {
        // positions         // texture Coords
        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
        
        0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  1.0f
    };
    
    
    // transparent VAO
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void BlendRender::setupBlend() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void BlendRender::setup() {
    setupBlend();
    setDepthTest();
    setupCube();
    setupPlane();
    setupTransparentVAO();
    loadTex();
    
    // build and compile shaders
    // -------------------------
    _shader = new Shader(ResPath("/code_resource/blend/blend_vertex.vs"),
                         ResPath("/code_resource/blend/blend_fragment.fs"));

    _shader->use();
    _shader->setInt("texture1", 0);
    
    
    // transparent  locations
    // --------------------------------
    transparentPose =
    {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3( 1.5f, 0.0f, 0.51f),
        glm::vec3( 0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3 (0.5f, 0.0f, -0.6f)
    };
}

void BlendRender::loadTex() {
    DepthTestRender::loadTex();
    loadTexture(ResPath("/code_resource/blend/grass.png"), &vegetationTexture, GL_RGB);
    loadTexture(ResPath("/code_resource/blend/blending_transparent_window.png"), &windowTexture, GL_RGB);
}

void BlendRender::render() {
    DepthTestRender::render();
//    renderVegetation();
    renderWindow();
}

void BlendRender::renderVegetation() {
    // 需要修改shader，开启`if(texColor.a < 0.1) discard;`
    glBindVertexArray(transparentVAO);
    glBindTexture(GL_TEXTURE_2D, vegetationTexture);
    for (unsigned int i = 0; i < transparentPose.size(); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, transparentPose[i]);
        _shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void BlendRender::renderWindowsDepthError() {
    glBindVertexArray(transparentVAO);
    glBindTexture(GL_TEXTURE_2D, windowTexture);
    for (unsigned int i = 0; i < transparentPose.size(); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, transparentPose[i]);
        _shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void BlendRender::renderWindow() {
    // sort the transparent windows before rendering
    // ---------------------------------------------
    std::map<float, glm::vec3> sorted;
    for (unsigned int i = 0; i < transparentPose.size(); i++)
    {
        float distance = glm::length(_camera->Position - transparentPose[i]);
        sorted[distance] = transparentPose[i];
    }
    
    // windows (from furthest to nearest)
    glBindVertexArray(transparentVAO);
    glBindTexture(GL_TEXTURE_2D, windowTexture);
    for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, it->second);
        _shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
