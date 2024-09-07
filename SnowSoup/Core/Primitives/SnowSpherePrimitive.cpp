//
//  SnowSpherePrimitive.cpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 06/09/24.
//

#include "SnowSpherePrimitive.hpp"

SnowSpherePrimitive::SnowSpherePrimitive(MTL::Device* device){
    this->device = device;
}

void SnowSpherePrimitive::Sphere(){
    UInt16 segments = 16;
    UInt16 rings = 8;
    float radius = 10;
    
    UInt16 verticesCount = segments * rings + 1;
    UInt16 indicesCount = ((rings - 2) * (segments - 1) * 6 + 2*(segments - 1) * 3 );
    
    simd::float3 positions[verticesCount];
    
    simd::float3 normals[verticesCount];
    
    UInt32 indices[indicesCount];
    
    simd::float3 color[] =
    //color
    {
    {1.0, 0.0, 0.0}
    };
    
    for (UInt16 i = 1; i < rings; i++) {
        float valueI = i / (float) rings;
        float angleI = valueI * SNOWSOUP_PI_F;
        float y = cos(angleI);
        float mult = sin(angleI);
        
        for (UInt16 j = 0; j < segments; j++) {
            float valueJ = j / (float) (segments - 1);
            float angleJ = valueJ * SNOWSOUP_TAU;
            float x = cos(angleJ) * mult;
            float z = -sin(angleJ) * mult;
            
            positions[j + (i - 1) * segments] = simd::float3 {x * radius, y * radius, z * radius};
            
            normals[j + (i - 1) * segments] = simd::float3 {x, y, z};
        }
    }
    
    UInt16 top = (UInt16) verticesCount - 2;
    positions[verticesCount - 2 ] = simd::float3 {0, radius, 0};
    normals[verticesCount - 2 ] = simd::float3 { 0, 1, 0 };
    
    UInt16 bot = (UInt16) verticesCount - 1;
    positions[verticesCount - 1 ] = simd::float3 {0, -radius, 0};
    normals[verticesCount - 1 ] = simd::float3 { 0, -1, 0 };
    
    for (UInt16 i = 0; i < rings - 2; i++) {
        for (UInt16 j = 0; j < segments - 1; j++) {
            UInt32 one = (i * segments) + j;
            UInt32 two = ((i + 1) * segments) + j;
            UInt32 three = ((i + 1) * segments) + j + 1;
            UInt32 four = (i * segments) + j + 1;
            
            int index = (i * (segments - 1) + j) * 6;
            
            indices[index] = one;
            indices[index + 1] = two;
            indices[index + 2] = three;
            
            indices[index + 3] = one;
            indices[index + 4] = three;
            indices[index + 5] = four;
        }
    }
    
    for (UInt16 i = 0; i < segments - 1; i++) {
        indices[indicesCount - 6 * (segments - 1) + i * 3] = i;
        indices[indicesCount - 6 * (segments - 1) + i * 3 + 1] = i + 1;
        indices[indicesCount - 6 * (segments - 1) + i * 3 + 2] = top;
    }
    
    for (UInt16 i = 0; i < segments - 1; i++) {
        UInt32 offset = (rings - 2) * segments;
        
        indices[indicesCount - 3 * (segments - 1) + i * 3 ] = offset + i;
        indices[indicesCount - 3 * (segments - 1) + i * 3 + 1] = bot;
        indices[indicesCount - 3 * (segments - 1) + i * 3 + 2] = offset + i + 1;
    }

    const size_t positionsDataSize = sizeof(positions);
    const size_t colorsDataSize = sizeof(color);
    const size_t normalsDataSize = sizeof(normals);
    const size_t indexDataSize = sizeof(indices);
    
    numI = indicesCount;

    MTL::Buffer* pVertexPositionsBuffer = device->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged);
    MTL::Buffer* pVertexColorsBuffer = device->newBuffer(colorsDataSize, MTL::ResourceStorageModeManaged);
    MTL::Buffer* pVertexNormalsBuffer = device->newBuffer(normalsDataSize, MTL::ResourceStorageModeManaged);
    
    MTL::Buffer* index = device->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

    _pVertexPositionsBuffer = pVertexPositionsBuffer;
    _pVertexColorsBuffer = pVertexColorsBuffer;
    _pVertexNormalsBuffer = pVertexNormalsBuffer;
    
    _pIndexBuffer = index;

    memcpy( _pVertexPositionsBuffer->contents(), positions, positionsDataSize );
    memcpy( _pVertexNormalsBuffer->contents(), normals, normalsDataSize );
    memcpy( _pVertexColorsBuffer->contents(), color, colorsDataSize );
    memcpy( _pIndexBuffer->contents(), indices, indexDataSize );

    _pVertexPositionsBuffer->didModifyRange( NS::Range::Make( 0, _pVertexPositionsBuffer->length() ) );
    _pIndexBuffer->didModifyRange(NS::Range::Make(0, _pIndexBuffer->length()));
    _pVertexNormalsBuffer->didModifyRange(NS::Range::Make(0, _pVertexNormalsBuffer->length()));
    _pVertexColorsBuffer->didModifyRange( NS::Range::Make( 0, _pVertexColorsBuffer->length() ) );
}

void SnowSpherePrimitive::Draw(MTL::RenderCommandEncoder* pEnc, Snow_Uniforms* uniforms, Snow_SkyboxUniforms* skyboxUniforms) {
    pEnc->setVertexBuffer(_pVertexPositionsBuffer, 0, 0);
    pEnc->setVertexBuffer(_pVertexNormalsBuffer, 0, 1);
    pEnc->setVertexBuffer(_pVertexColorsBuffer, 0, 2);
    pEnc->setVertexBytes(uniforms, sizeof(Snow_Uniforms), NS::UInteger(3));
    pEnc->setFragmentBytes(skyboxUniforms, sizeof(Snow_SkyboxUniforms), NS::UInteger(0));
    
    pEnc->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, numI, MTL::IndexTypeUInt32, _pIndexBuffer, 0);
    pEnc->setFrontFacingWinding(MTL::WindingCounterClockwise);
}
