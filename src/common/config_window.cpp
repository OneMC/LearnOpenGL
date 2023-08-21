//
//  config_window.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/8/15.
//

#include "config_window.hpp"
#include <iostream>
#include "render.hpp"

using namespace std;

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;
extern Render* render;

GLFWwindow * setup_context() {
    //GLFW
    glfwInit();
    
    //OpenGL(Major)(Minor)3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //(Core-profile)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return NULL;
    }
    //GLFW
    glfwMakeContextCurrent(window);
    
    //GLADOpenGLOpenGLGLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Failed to initialize GLAD" << endl;
        return NULL;
    }
    
    //GLFW
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    return window;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    render->mouse_callback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    render->scroll_callback(window, xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow * window,int width,int height) {
    glViewport(0,0,width,height);
}

void process_input(GLFWwindow * window) {
    //(Esc) glfwGetKey GLFW_RELEASEGLFW_PRESS
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    render->process_input(window);
}
