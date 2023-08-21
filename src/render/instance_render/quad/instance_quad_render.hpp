//
//  instance_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/28.
//

#ifndef instance_render_hpp
#define instance_render_hpp

#include "camera_base_render.hpp"

class InstanceQuadRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~InstanceQuadRender(){}

private:
    unsigned int instanceVBO=0, quadVAO=0;
    void setupInstance();
};

#endif /* instance_render_hpp */
