//
//  ibl_irradiance_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/8/15.
//

#ifndef ibl_irradiance_render_hpp
#define ibl_irradiance_render_hpp

#include "ibl_irradiance_conversion_render.hpp"

class IBLIrradianceRender: public IBLIrradianceConvRender {
public:
    void setup() override;
    void render() override;
    ~IBLIrradianceRender(){}

protected:
    unsigned int irradianceMap=0;
    Shader irradianceShader;
    void setupCommonShader() override;
    void commonSetup() override;
    void setupShader() override;
    void createCubemap();
};

#endif /* ibl_irradiance_render_hpp */
