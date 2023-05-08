//
//  CameraRender.hpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/4/30.
//

#ifndef CameraRender_hpp
#define CameraRender_hpp

#include <stdio.h>
#include "Render.hpp"
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class CameraRender: public Render
{
public:
    virtual void processInput(GLFWwindow *window) override;
    virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
    virtual void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) override;
    
    virtual void setup() override;
    virtual void render() override;
    ~CameraRender();
private:
    Shader *_shader = nullptr;
};


#endif /* CameraRender_hpp */
