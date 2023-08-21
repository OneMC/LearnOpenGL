//
//  model_render.hpp
//  LearnOpenGL
//
//  Created by MC on 2023/7/13.
//

#ifndef model_render_hpp
#define model_render_hpp

#include "camera_base_render.hpp"
#include "model.h"

class ModelRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~ModelRender() = default;

private:
    Model model;
};

#endif /* model_render_hpp */
