//
//  config_window.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/8/15.
//

#ifndef config_window_hpp
#define config_window_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow * setup_context();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

#endif /* config_window_hpp */
