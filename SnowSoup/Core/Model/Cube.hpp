//
//  Cube.hpp
//  SnowSoup
//
//  Created by Gustavo Binder on 03/10/24.
//

#ifndef Cube_hpp
#define Cube_hpp

#include <Metal/Metal.hpp>
#include <simd/simd.h>

#include "Core/Transform/Transform.hpp"

class Cube : public Transform {
public:
    Cube(MTL::Device* device) {
        this->device = device;
        isPrimitive = true;
    }
    
    void Draw( MTL::RenderCommandEncoder* pEnc, Snow_Uniforms* uniforms, Snow_PhongUniforms* phongUniforms ) override;
    
    MTL::Device* device;
    MTL::Buffer* vertexBuffer;
    MTL::Buffer* indexBuffer;
    MTL::Buffer* normalBuffer;
    MTL::Buffer* texvtxBuffer;
    
    simd_float3 color = {
        1.f, 0.f, 0.f
    };
    
    float s = 0.5f;
    
    simd::float3 vertices[8] = {
        {s, s, s},
        {-s, s, s},
        {-s, s, -s},
        {s, s, -s},
        {s, -s, s},
        {-s, -s, s},
        {-s, -s, -s},
        {s, -s, -s},
    };
    
    UInt32 indices[24] = {
        0, 1,   1, 2,   2, 3,   3, 0,
        0, 4,   1, 5,   2, 6,   3, 7,
        4, 5,   5, 6,   6, 7,   7, 4
    };
    
    void buildBuffers();
    void setColor(float r, float g, float b);
};

#endif /* Cube_hpp */
