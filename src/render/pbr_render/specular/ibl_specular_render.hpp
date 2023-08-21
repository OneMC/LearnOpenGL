//
//  ibl_specular_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/8/17.
//

#ifndef ibl_specular_render_hpp
#define ibl_specular_render_hpp

#include "ibl_irradiance_render.hpp"

class IBLSpecularRender: public IBLIrradianceRender {
public:
    void setup() override;
    void render() override;

protected:
    unsigned int quadVAO=0;
    unsigned int prefilterMap=0,brdfLUTTexture=0;
    Shader prefilterShader;
    Shader brdfShader;
    void commonSetup() override;
    void setupShader() override;
    void setupCommonShader() override;
    void setupEnvCubemap() override;
    void setupCubemap() override;
    void setupPrefilter();
    void setupBRDF();
};

#endif /* ibl_specular_render_hpp */
