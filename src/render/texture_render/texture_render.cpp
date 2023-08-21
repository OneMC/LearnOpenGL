//
//  texturerender.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/6.
//

#include "texture_render.hpp"

extern float TextureRenderVertices[32];

void TextureRender::setupVertex() {
    unsigned int TextureRenderIndices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TextureRenderVertices), TextureRenderVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TextureRenderIndices), TextureRenderIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0); // unbind
}

void TextureRender::setup() {
    setupVertex();
    
    // The loader order has nothing to do with the shader's texture order
    containerT = load2DTexture(R::image("container.jpg"), true, GL_CLAMP_TO_EDGE);
    awesomeT = load2DTexture(R::image("awesomeface.png"));
    
    shader = Shader(R::shader("texture_render/texture_vertex.vsh"),
                    R::shader("texture_render/texture_fragment.fsh"));
    
    {
        // If there are multiple shader program, the following code needs to set every time
        
        // 0: use shader
        shader.use();
        
        // 1: set 'randomName1' to GL_TEXTURE0
        shader.setInt("randomName1", 0);
        // 2: bind 'awesomeT' to GL_TEXTURE1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, awesomeT);
        
        // 1: set 'randomName2' to GL_TEXTURE1
        shader.setInt("randomName2", 1);
        // 2: bind 'containerT' to GL_TEXTURE0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, containerT);
    }
}

void TextureRender::render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glCheckError();
}
