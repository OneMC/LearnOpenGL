//
//  deferred_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/25.
//

#ifndef deferred_render_hpp
#define deferred_render_hpp

#include "camera_base_render.hpp"
#include "model.h"

class DeferredRender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    ~DeferredRender(){}
    
protected:
    unsigned int quadVAO = 0, cubeVAO=0;
    unsigned int gBuffer=0;
    unsigned int gPosition=0, gNormal=0, gAlbedoSpec=0;
    
    Shader geometryShader;
    Shader lightingShader;
    Shader lightBoxShader;
    
    Model backpack;
    std::vector<glm::vec3> objectPositions;
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;
    
    virtual void setupLightingShader();
    virtual void passLightInfoToShader();
    
    void setupGBuffer();
    void loadModel();
    void setupLight();
    
    void renderToGeometryBuffer();
    void renderLightWithGBuffer();
    void renderLight();
};

class DeferredVolumeRender: public DeferredRender {
private:
    void setupLightingShader() override;
    void passLightInfoToShader() override;
};

#endif /* deferred_render_hpp */
