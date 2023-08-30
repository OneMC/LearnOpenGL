//
//  camera_base_render.hpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/7.
//

#ifndef camera_base_render_hpp
#define camera_base_render_hpp

#include "render.hpp"
#include "camera.h"

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

class CameraBaseRender: public Render {

public:
    void framebuffer_size_callback(GLFWwindow* window, int width, int height) override;
    void mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
    void process_input(GLFWwindow *window) override;

    ~CameraBaseRender(){}
    
protected:
    bool firstMouse = true;
    // timing
    float deltaTime = 0.1f;
    float lastFrame = 0.0f;
    float lastX =  800.0f / 2.0;
    float lastY =  600.0 / 2.0;
    
    Camera camera = Camera(glm::vec3(0.0f, 1.0f, 10.0f));
    
    void updateCamera();
    glm::mat4 viewModel();
    virtual glm::mat4 projectionModel();
};

#endif /* camera_base_render_hpp */
