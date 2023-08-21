//
//  cubemap_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/24.
//

#ifndef cubemap_render_hpp
#define cubemap_render_hpp

#include "camera_base_render.hpp"
class CubeMapRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~CubeMapRender(){}

protected:
    unsigned int skyboxVAO=0;
    unsigned int cubeTexture=0, cubemapTexture=0;
    Shader skyboxShader;
    
    virtual void setupSceneShader();
    virtual void renderScene();
    void renderSkybox();
};

class EnvrMapRender: public CubeMapRender {
private:
    void setupSceneShader() override;
    void renderScene() override;
    ~EnvrMapRender(){}
};

#endif /* cubemap_render_hpp */
