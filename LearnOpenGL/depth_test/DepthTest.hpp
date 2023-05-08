//
//  DepthTest.hpp
//  LearnOpenGL
//
//  Created by 苗超 on 2023/4/17.
//

#ifndef DepthTest_hpp
#define DepthTest_hpp

#include "AbstractCamera.hpp"
#include "shader.h"

class DepthTestRender : public CamerRenderA {
public:
    virtual void setup() override;
    virtual void render() override;
    virtual void releaseRes() override;
    ~DepthTestRender(){}
    
protected:
    Shader *_shader = nullptr;
    unsigned int cubeVAO;
    unsigned int cubeVBO;
    unsigned int planeVAO;
    unsigned int planeVBO;
    unsigned int cubeTexture;
    unsigned int floorTexture;
    virtual void setupCube();
    virtual void setupPlane();
    virtual void setDepthTest();
    virtual void loadTex();
    void renderCube();
    void renderFloor();
    
    virtual void setupCamera() override;
};

#endif /* DepthTest_hpp */
