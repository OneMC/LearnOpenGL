//
//  pbr_irradiance_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/8/15.
//

#ifndef ibl_irradiance_conv_render_hpp
#define ibl_irradiance_conv_render_hpp

#include "pbr_lighting_render.hpp"
#include "shader_tool.hpp"

class IBLIrradianceConvRender: public PBRLightRender {
public:
    void setup() override;
    void render() override;
    ~IBLIrradianceConvRender(){}

protected:
    unsigned int cubeVAO=0;
    Shader equirectangularToCubemapShader, backgroundShader;
    virtual void setupCommonShader();
    virtual void setupShader();
    
    unsigned int hdrTexture=0, envCubemap=0;
    unsigned int captureFBO=0, captureRBO=0;
    void loadHdrTexture();
    void setupFrameBuffer();
    virtual void setupCubemap();
    virtual void setupEnvCubemap();
    void setupStaticShader();
    virtual void commonSetup();
    
    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[6] =  {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
};

#endif /* pbr_irradiance_render_hpp */
