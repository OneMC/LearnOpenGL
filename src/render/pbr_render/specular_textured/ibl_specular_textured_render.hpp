//
//  ibl_specular_textured_render.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/8/18.
//

#ifndef ibl_specular_textured_render_hpp
#define ibl_specular_textured_render_hpp

#include "ibl_specular_render.hpp"

class IBLSpecularTexturedRender: public IBLSpecularRender {
public:
    void setup() override;
    void render() override;
    ~IBLSpecularTexturedRender(){}

private:
    void setupShader() override;
    void loadTexture();
    
    // rusted iron
    unsigned int ironAlbedoMap=0,ironNormalMap=0,
    ironMetallicMap=0, ironRoughnessMap=0, ironAOMap=0;

    // gold
    unsigned int goldAlbedoMap=0, goldNormalMap=0,
    goldMetallicMap=0, goldRoughnessMap=0, goldAOMap=0;

    // grass
    unsigned int grassAlbedoMap=0, grassNormalMap=0,
    grassMetallicMap=0, grassRoughnessMap=0,grassAOMap=0;

    // plastic
    unsigned int plasticAlbedoMap=0, plasticNormalMap=0,
    plasticMetallicMap=0, plasticRoughnessMap=0, plasticAOMap=0;

    // wall=0,
    unsigned int wallAlbedoMap=0, wallNormalMap=0,
    wallMetallicMap=0, wallRoughnessMap=0,wallAOMap=0;

};

#endif /* ibl_specular_textured_render_hpp */
