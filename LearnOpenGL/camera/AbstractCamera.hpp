//
//  AbstractCamera.hpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/4/30.
//

#ifndef AbstractCamera_hpp
#define AbstractCamera_hpp

#include <stdio.h>
#include "Render.hpp"
#include "camera.hpp"

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

class CamerRenderA: public Render {
public:
    virtual void framebuffer_size_callback(GLFWwindow* window, int width, int height) override;
    virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
    virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
    virtual void processInput(GLFWwindow *window) override;
    ~CamerRenderA(){}
protected:
    bool firstMouse = true;
    // timing
    float deltaTime = 0.1f;
    float lastFrame = 0.0f;
    float lastX =  800.0f / 2.0;
    float lastY =  600.0 / 2.0;

    virtual void setupCamera();
    Camera *_camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    glm::mat4 viewModel();
    glm::mat4 projectionModel();
};

#endif /* AbstractCamera_hpp */
