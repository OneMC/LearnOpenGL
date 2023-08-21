//
//  ssao_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/8/2.
//

#ifndef ssao_render_hpp
#define ssao_render_hpp

#include "camera_base_render.hpp"
#include "model.h"

class SSAORender: public CameraBaseRender {
public:
    void setup() override;
    void render() override;
    glm::mat4 projectionModel() override;
    ~SSAORender(){}

private:
    unsigned int cubeVAO=0, quadVAO=0;
    
    // lighting info
    glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
    glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);
    
    unsigned int gPosition, gNormal, gAlbedo;
    unsigned int noiseTexture;
    unsigned int gBuffer=0;
    unsigned int ssaoFBO=0, ssaoBlurFBO=0;
    std::vector<glm::vec3> ssaoKernel;
    unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
    
    Shader shaderGeometryPass;
    Shader shaderLightingPass;
    Shader shaderSSAO;
    Shader shaderSSAOBlur;
    
    Model backpack;
    
    void setupShader();
    void setupGBuffer();
    void setupSSAOBuffer();
    void setupNoiseTexture();
    
    void geometryRender();
    void handleTexture();
    void lightingRender();
};

#endif /* ssao_render_hpp */
