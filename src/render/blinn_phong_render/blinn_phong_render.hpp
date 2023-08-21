//
//  blinn_phong_render.hpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/13.
//

#ifndef blinn_phong_render_hpp
#define blinn_phong_render_hpp

#include "camera_base_render.hpp"

class BlinnPhongRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    void process_input(GLFWwindow *window) override;
    ~BlinnPhongRender(){}

private:
    unsigned int planeVAO = 0;
    unsigned int floorTexture = 0;
    bool blinn = false;
    bool blinnKeyPressed = false;
};

#endif /* blinn_phong_render_hpp */
