//
//  blend_render.cpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/10.
//

#include "blend_render.hpp"
#include "shader_tool.hpp"
#include <map>

using namespace std;

void BlendRender::setup() {
    DepthTestRender::setup();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    setupTransparentVertex();
    
    shader = Shader(R::shader("depth_test_render/depth_test_vertex.vsh"),
                    R::shader("blend_render/blend_fragment.fsh"));
    
    vegetationTexture = load2DTexture(R::image("grass.png"));
    windowTexture = load2DTexture(R::image("window.png"));
    
    // transparent window locations
    objectsLocation = {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3( 1.5f, 0.0f, 0.51f),
        glm::vec3( 0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3( 0.5f, 0.0f, -0.6f)
    };
}

void BlendRender::setupTransparentVertex() {
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

void BlendRender::render() {
    DepthTestRender::render();
//    renderVegetation();
//    renderWindowsDepthError();
    renderWindow();
}

void BlendRender::renderVegetation() {
    // 需要修改shader，开启`if(texColor.a < 0.1) discard;`
    glBindVertexArray(transparentVAO);
    glBindTexture(GL_TEXTURE_2D, vegetationTexture);
    for (unsigned int i = 0; i < objectsLocation.size(); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, objectsLocation[i]);
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);
}

void BlendRender::renderWindowsDepthError() {
    glBindVertexArray(transparentVAO);
    glBindTexture(GL_TEXTURE_2D, windowTexture);
    for (unsigned int i = 0; i < objectsLocation.size(); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, objectsLocation[i]);
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void BlendRender::renderWindow() {
    // sort the transparent windows before rendering
    map<float, glm::vec3> sorted;
    for (unsigned int i = 0; i < objectsLocation.size(); i++)
    {
        float distance = glm::length(camera.Position - objectsLocation[i]);
        sorted[distance] = objectsLocation[i];
    }
    
    // windows (from furthest to nearest)
    glBindVertexArray(transparentVAO);
    glBindTexture(GL_TEXTURE_2D, windowTexture);
    for (map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, it->second);
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);
}
