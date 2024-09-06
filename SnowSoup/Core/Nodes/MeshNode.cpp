//
//  MeshNode.cpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 03/09/24.
//

#include "MeshNode.hpp"

void MeshNode::extractRotation(){
    simd::float4x4 rotationMatrix = transformation;
    // Zeroes position element
    rotationMatrix.columns[3] = { 0, 0, 0, 1 };
    
    // Extracts scale element
    float sx = simd_length(rotationMatrix.columns[0].xyz);
    float sy = simd_length(rotationMatrix.columns[1].xyz);
    float sz = simd_length(rotationMatrix.columns[2].xyz);
    
    // Divides matrix by scale resulting in rotation matrix
    rotationMatrix.columns[0] /= sx;
    rotationMatrix.columns[1] /= sy;
    rotationMatrix.columns[2] /= sz;
    
    this->rotationMatrix = rotationMatrix;
}
