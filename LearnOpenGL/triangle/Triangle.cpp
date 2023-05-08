//
//  Triangle.cpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/3/23.
//

#include "Triangle.hpp"
#include <iostream>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static unsigned int VAO_pos_color;
static unsigned int VAO_position;
static unsigned int VAO_for_ebo;

static unsigned int vertexShader = -1;
static unsigned int fragmentShader = -1;
unsigned int triangle_shaderProgram = -1;

const char *vertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";


void compileShader() {
    // vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // check for vertext shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // check for fragment shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void linkProgram() {
    int success;
    char infoLog[512];
    // link shaders
    triangle_shaderProgram = glCreateProgram();
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
}

void setup_VAO_only_position() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    
    unsigned int VBO;
    glGenBuffers(1, &VBO); // 生成VBO句柄
    glGenVertexArrays(1, &VAO_position);
    
    /*
     首先要初始化VAO: `glBindVertexArray`
     在这之后通过`glBindBuffer`生成的VBO都会绑定到VAO中
     */
    glBindVertexArray(VAO_position);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    /*
     Copy data from CPU to GPU
     1. Buffer Type: GL_ARRAY_BUFFER
     2. data size
     3. data
     4. how to manage data:
        GL_STREAM_DRAW: set once, use few times
        GL_STATIC_DRAW: set once, use many times
        GL_DYNAMIC_DRAW: change & use many times
        这个参数影响GPU如何管理内存
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
    
    glBindVertexArray(0); // unbind VAO
}

void setup_VAO_position_color() {
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right / red
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left  / green
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top          / blue

    };
    glGenVertexArrays(1, &VAO_pos_color);
    glBindVertexArray(VAO_pos_color);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0); // unbind VAO
}

void setup_EBO() {
    /*
    GLfloat vertices[] = {
        // First triangle
        0.5f,  0.5f,  // Top Right
        0.5f, -0.5f,  // Bottom Right
        -0.5f,  0.5f,  // Top Left
        // Second triangle
        0.5f, -0.5f,  // Bottom Right
        -0.5f, -0.5f,  // Bottom Left
        -0.5f,  0.5f   // Top Left
    };
    */
    
    GLfloat vertices[] = {
        0.5f,  0.5f, 0.0f,  // Top Right
        0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  // Bottom Left
        -0.5f,  0.5f, 0.0f   // Top Left
    };
    
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glGenVertexArrays(1, &VAO_for_ebo);
    glBindVertexArray(VAO_for_ebo);
    
    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    
    // Uncommenting this call will result in wireframe polygons.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void setup_triangle() {
    compileShader();
    linkProgram();
    setup_VAO_position_color();
    setup_VAO_only_position();
}

void render_triangle() {
    glUseProgram(triangle_shaderProgram);
    glBindVertexArray(VAO_pos_color);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void setup_rectangle() {
    compileShader();
    linkProgram();
    setup_EBO();
}

void render_rectangle() {
    // Draw our first triangle
    glUseProgram(triangle_shaderProgram);
    glBindVertexArray(VAO_for_ebo);
    
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Triangle::setup() {
    setup_rectangle();
}

void Triangle::render() {
    render_rectangle();
}
