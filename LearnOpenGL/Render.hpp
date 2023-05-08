//
//  Render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/3/23.
//

#ifndef Render_hpp
#define Render_hpp

extern const char * ProjectPath;
#define ResPath(file) (std::string(ProjectPath) + std::string(file)).c_str()

#include <stdio.h>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Render {
public:
    virtual void setup() {}
    virtual void render() {}
    virtual void releaseRes(){}
    virtual void framebuffer_size_callback(GLFWwindow* window, int width, int height){}
    virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos){}
    virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){}
    virtual void processInput(GLFWwindow *window){}
    virtual ~Render(){}
};

#endif /* Render_hpp */
