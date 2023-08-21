//
//  depth_test_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/7.
//

#ifndef depth_test_render_hpp
#define depth_test_render_hpp

#include "camera_base_render.hpp"

class DepthTestRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    
    ~DepthTestRender(){}
    
protected:
    unsigned int planeVAO=0;
    unsigned int cubeTexture = 0, floorTexture=0;
    void setupDepthTest();
    void setupCamera();
    void renderCube();
    void renderFloor();
};

#endif /* depth_test_render_hpp */
