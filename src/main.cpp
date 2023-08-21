//
//  main.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/3.
//

#include <iostream>
#include "config_window.hpp"
#include "renderer_collection.h"

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
Render* render;

int main() {
    GLFWwindow* window = setup_context();
    if (window == nullptr) return -1;

    render = new IBLSpecularTexturedRender();
    render->window = window;
    render->setup();

    // Render Loop
    while (!glfwWindowShouldClose(window))  {
        process_input(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        render->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    render->release();
    glfwTerminate();
    delete render;
    return 0;
}
