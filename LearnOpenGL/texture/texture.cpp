//
//  texture.cpp
//  TestOpengl
//
//  Created by 苗超 on 2023/3/7.
//

#include "texture.hpp"
#include "shader.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static unsigned int VBO, VAO, EBO;
static unsigned int texture1, texture2;

void loadTexture(char const * texturePath, unsigned int *texture, int colorType) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    // load image data
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture" << std::endl;
        assert(false);
    }
    
    GLenum format;
    if(nrChannels == 1) {
        format = GL_RED;
    } else if (nrChannels == 3) {
        format = GL_RGB;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
    } else {
        format = GL_RGB;
    }
    
    glGenTextures(1, texture); // 生成纹理句柄
    glBindTexture(GL_TEXTURE_2D, *texture); // 感觉相当于初始化
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    
    /*
     Texture wrap: 处理纹理坐标不在0-1之间
     需要为每个方向都设置wrapping值，因为是2D图像，所以不用设置z轴
     属性:
        - GL_TEXTURE_WRAP_S x轴
        - GL_TEXTURE_WRAP_T y轴
        - GL_TEXTURE_WRAP_R z轴
     值:
        - GL_REPEAT
        - GL_MIRRORED_REPEAT
        - GL_CLAMP_TO_EDGE
        - GL_CLAMP_TO_BORDER
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    
    /*
    // Only for `GL_CLAMP_TO_BORDER`
    // set border color
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
     */
    
    /*
     Texture filter: 处理texture分辨率过低
     属性:
        - GL_TEXTURE_MAG_FILTER
     值:
        - GL_NEAREST
        - GL_LINEAR
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    /*
     Texture filter: 处理texture分辨率过高
     属性:
        - GL_TEXTURE_MIN_FILTER
     值:
      - GL_NEAREST_MIPMAP_NEAREST
      - GL_LINEAR_MIPMAP_NEAREST
      - GL_NEAREST_MIPMAP_LINEAR
      - GL_LINEAR_MIPMAP_LINEAR
     如果将以上属性设置给`GL_TEXTURE_MAG_FILTER`产生`GL_INVALID_ENUM`错误
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    /*
     Create texture: Copy image to GPU data
     params:
     1: GL_TEXTURE_1D / GL_TEXTURE_2D / GL_TEXTURE_3D
     2: set mipmap level: 0 base level
     3: texture stored format: GL_RGB-> rgb only
     4&5: width & height
     6: always 0
     7&8: image: format & datatype
     9: image data
     */
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    /*
     Create mipmap:
     1. call 'glTexImage2D' with different second argument: for special level
     2. call 'glGenerateMipmap': automatically generate all required level
     */
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // 看起来以上的‘glTexParameteri’函数会对最近生成的纹理生效
    stbi_image_free(data);
}

void setupVertexConfig() {
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };
    
    
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void transform_translate_demo() {
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f); // identity matrix
    
    /*
     glm::translate
     https://stackoverflow.com/questions/59222806/how-does-glm-handle-translation
             c0  c1  c2  c3
        ---------------------
        r0:   1   0   0  v[0]
        r1:   0   1   0  v[1]
        r2:   0   0   0  v[2]
        r3:   0   0   0  1
     */
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;
}

glm::mat4 transform_scale() {
    glm::mat4 trans = glm::mat4(1.0f);
    // 绕z轴旋转90度
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    // xyz都缩放0.5
    return glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
}

void Texture::setup() {
    setupVertexConfig();
    loadTexture(ResPath("/LearnOpenGL/texture/container.jpeg"), &texture1,GL_RGB);
    loadTexture(ResPath("/LearnOpenGL/texture/awesomeface.png"), &texture2,GL_RGBA);
    _shader = new Shader(ResPath("/LearnOpenGL/texture/texture_vertex.vs"),
                         ResPath("/LearnOpenGL/texture/texture_fragment.fs"));
    _shader->use();
    
    // GLM和OpenGL都是column-major
//    unsigned int transformLoc = glGetUniformLocation(_shader.ID, "transform");
//    glm::mat4 trans = transform_scale();
//    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void Texture::render() {
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    // --- 设置texture unit ---
    // 方式1: texture1对应着fragment shader中的texture1变量
    glUniform1i(glGetUniformLocation(_shader->ID, "texture1"), 0);
    // 方式2: texture2对应着fragment shader中的texture2变量
    _shader->setInt("texture2", 1);
    
    // create transformations
    glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    
    // transform顺序：首先用rotate然后是translate
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // get matrix's uniform location and set matrix
    _shader->use();
    unsigned int transformLoc = glGetUniformLocation(_shader->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    
    // render container
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Texture::~Texture() {
    delete _shader;
}
