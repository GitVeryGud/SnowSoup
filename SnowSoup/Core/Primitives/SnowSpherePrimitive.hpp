//
//  SnowSpherePrimitive.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 06/09/24.
//

#ifndef SnowSpherePrimitive_hpp
#define SnowSpherePrimitive_hpp

#include "SnowDefs.h"
#include <simd/simd.h>
#include <Metal/Metal.hpp>
#include "Core/Transform/Transform.hpp"

class SnowSpherePrimitive : public Transform{
public:
    SnowSpherePrimitive(MTL::Device* device);
    void Sphere();
    void Draw( MTL::RenderCommandEncoder* pEnc, Snow_Uniforms* uniforms, Snow_SkyboxUniforms* skyboxUniforms) override;
    
private:
    MTL::Device* device;
    MTL::Buffer* _pVertexPositionsBuffer;
    MTL::Buffer* _pIndexBuffer;
    MTL::Buffer* _pVertexNormalsBuffer;
    MTL::Buffer* _pVertexColorsBuffer;
    
    int numI;    
};

#endif /* SnowSpherePrimitive_hpp */
