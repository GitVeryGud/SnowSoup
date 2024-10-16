//
//  Cube.cpp
//  SnowSoup
//
//  Created by Gustavo Binder on 03/10/24.
//

#include "Cube.hpp"

void Cube::buildBuffers() {
    const size_t sizeOfVertices = sizeof(vertices);
    const size_t sizeOfIndices = sizeof(indices);
    
    vertexBuffer = device->newBuffer(sizeOfVertices, MTL::ResourceStorageModeManaged);
    indexBuffer = device->newBuffer(sizeOfIndices, MTL::ResourceStorageModeManaged);
    
    memcpy(vertexBuffer->contents(), vertices, sizeOfVertices);
    memcpy(indexBuffer->contents(), indices, sizeOfIndices);
    
    vertexBuffer->didModifyRange(NS::Range::Make(0, vertexBuffer->length()));
    indexBuffer->didModifyRange(NS::Range::Make(0, indexBuffer->length()));
}

void Cube::Draw(MTL::RenderCommandEncoder* pEnc, Snow_Uniforms* uniforms, Snow_PhongUniforms* phongUniforms) {
    pEnc->setVertexBuffer(vertexBuffer, 0, 0);
    
    uniforms->modelMatrix = TransformMatrix();
    pEnc->setVertexBytes(uniforms, sizeof(Snow_Uniforms), 1);
    pEnc->setFragmentBytes(&color, sizeof(simd_float3), 1);
    pEnc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeLine, 24, MTL::IndexTypeUInt32, indexBuffer, 0);
}

void Cube::setColor(float r, float g, float b) {
    color.x = r;
    color.y = g;
    color.z = b;
}
