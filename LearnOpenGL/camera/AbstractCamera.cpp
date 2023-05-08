//
//  CamerRenderA.cpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/4/30.
//

#include "AbstractCamera.hpp"

void CamerRenderA::setupCamera() {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

glm::mat4 CamerRenderA::viewModel() {
    return _camera->GetViewMatrix();
}

glm::mat4 CamerRenderA::projectionModel() {
    return glm::perspective(glm::radians(_camera->Zoom),
                            (float)SCR_WIDTH / (float)SCR_HEIGHT,
                            0.1f,
                            100.0f);
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void CamerRenderA::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        _camera->ProcessKeyboard(FORWARD, deltaTime);
    }
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        _camera->ProcessKeyboard(BACKWARD, deltaTime);
    }
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        _camera->ProcessKeyboard(LEFT, deltaTime);
    }
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        _camera->ProcessKeyboard(RIGHT, deltaTime);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void CamerRenderA:: framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void CamerRenderA:: mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    
    lastX = xpos;
    lastY = ypos;
    
    _camera->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void CamerRenderA:: scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    _camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

