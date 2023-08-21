//
//  triangleRender.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/4.
//

#include "triangle_render.hpp"
#include <iostream>

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // orange color, u can change any color
    "}\n\0";


// vertex shader
unsigned int TriangleRender::compileVertexShader(const char *program) {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &program, NULL);
    glCompileShader(vertexShader);
    
    // check for vertext shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return vertexShader;
}

unsigned int TriangleRender::compileFragmentShader(const char *program) {
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &program, NULL);
    glCompileShader(fragmentShader);
    
    // check for fragment shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return fragmentShader;
}

unsigned int TriangleRender::linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    int success;
    char infoLog[512];
    
    // link shaders
    unsigned int triangle_shaderProgram = glCreateProgram();
    glAttachShader(triangle_shaderProgram, vertexShader);
    glAttachShader(triangle_shaderProgram, fragmentShader);
    glLinkProgram(triangle_shaderProgram);
    
    // check for linking errors
    glGetProgramiv(triangle_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(triangle_shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return triangle_shaderProgram;
}

void TriangleRender::setupVertex() {
    
    float vertices[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f,  // left
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top
        // second triangle
         0.0f, -0.5f, 0.0f,  // left
         0.9f, -0.5f, 0.0f,  // right
         0.45f, 0.5f, 0.0f   // top
    };
    
//    float vertices[] = {
//        -0.5f, -0.5f, 0.0f, // left
//         0.5f, -0.5f, 0.0f, // right
//         0.0f,  0.5f, 0.0f  // top
//    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // 1. bind the Vertex Array Object first
    // 2. bind and set vertex buffer(s)
    // 3. configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /*
     Copy data from CPU to GPU
     1. Buffer Type: GL_ARRAY_BUFFER
     2. data size
     3. data
     4. GPU how to manage data:
        GL_STREAM_DRAW: set once, use few times
        GL_STATIC_DRAW: set once, use many times
        GL_DYNAMIC_DRAW: change & use many times
     */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*
     Linking Vertex Attributes: position attribute
     1. attribute index in shader, `layout (location = #)`
     2. vertex attribute item count
     3. vertex attribute item size
     4. normalized: Integer to float type; GL_FALSE for float
     5. stride: all attributes data length for one vertex
     6. offset: offset for one vertex in its all attributes data
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

void TriangleRender::setup() {
    vertexShader = compileVertexShader(vertexShaderSource);
    fragmentShader = compileFragmentShader(fragmentShaderSource);
    shaderProgram = linkProgram(vertexShader, fragmentShader);
    setupVertex();
    glCheckError();
}

void TriangleRender::render() {
    // draw our first triangle
    glUseProgram(shaderProgram);
    // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glBindVertexArray(VAO);
    
    // 6 or 3 = vertices.count(setupVertex)
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError();
}

void TriangleRender_EBO::setupVertex() {
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void TriangleRender_EBO::render() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
