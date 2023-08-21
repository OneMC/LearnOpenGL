#pragma once
#include "render.hpp"

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

class CameraContainerRender: public Render {
public:
    void setup() override;
    void render() override;
    
    void framebuffer_size_callback(GLFWwindow* window, int width, int height) override;
    void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
    void process_input(GLFWwindow *window) override;

protected:
    // camera
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
    
    // yaw is initialized to -90.0 degrees
    // since a yaw of 0.0 results in a direction vector pointing to the right
    // so we initially rotate a bit to the left.
    float yaw   = -90.0f;	
    float pitch =  0.0f;
    float lastX =  float(SCR_WIDTH) / 2.0;
    float lastY =  float(SCR_HEIGHT) / 2.0;
    float fov   =  45.0f;

    bool firstMouse = true;
    
    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

private:
    void setupVertex();
    unsigned int containerT=0, awesomeT=0;
};
