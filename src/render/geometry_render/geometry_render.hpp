//
//  geometry_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/28.
//

#ifndef geometry_render_hpp
#define geometry_render_hpp

#include "camera_base_render.hpp"
#include "model.h"

class GeometryRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~GeometryRender(){}
};

class GeometryExplodingRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~GeometryExplodingRender(){}
    
private:
    Model nanosuit;
};

class GeometryNormalVizRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~GeometryNormalVizRender(){}

private:
    Shader normalShader;
    Model backpack;
};

#endif /* geometry_render_hpp */
