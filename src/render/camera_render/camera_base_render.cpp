//
//  camera_base_render.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/7.
//

#include "camera_base_render.hpp"

void CameraBaseRender::updateCamera() {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

glm::mat4 CameraBaseRender::viewModel() {
    return camera.GetViewMatrix();
}

glm::mat4 CameraBaseRender::projectionModel() {
    return glm::perspective(glm::radians(camera.Zoom),
                            (float)SCR_WIDTH / (float)SCR_HEIGHT,
                            0.1f,
                            100.0f);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void CameraBaseRender::process_input(GLFWwindow *window) {
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void CameraBaseRender:: framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void CameraBaseRender:: mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
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
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void CameraBaseRender:: scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
     camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

