//
//  Render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/3.
//

#ifndef Render_hpp
#define Render_hpp

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "debug_opengl.hpp"
#include "shader.h"
#include "resource.hpp"
#include "texture.hpp"

class Render {
public:
    GLFWwindow* window;
    
    virtual void setup() {}
    virtual void render() {}
    
    virtual void framebuffer_size_callback(GLFWwindow* window, int width, int height){}
    virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos){}
    virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){}
    virtual void process_input(GLFWwindow *window){}
    
    virtual void release(){}
    virtual ~Render() = default;

protected:
    unsigned int VBO=0, VAO=0, EBO=0;
    Shader shader;
};
#endif /* Render_hpp */
