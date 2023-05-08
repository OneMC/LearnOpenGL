//
//  blinnePhone.hpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/4/30.
//

#ifndef blinnePhone_hpp
#define blinnePhone_hpp

#include "AbstractCamera.hpp"

class BlinnPhongRender: public CamerRenderA {
public:
    virtual void setup() override;
    virtual void render() override;
    virtual void processInput(GLFWwindow *window) override;
    ~BlinnPhongRender(){}

protected:
    Shader *_shader = nullptr;
    unsigned int planeVAO;
    unsigned int floorTexture;
    void setupPlane();
    
private:
    virtual void setupCamera() override;
    bool blinn = false;
    bool blinnKeyPressed = false;
};

#endif /* blinnePhone_hpp */
