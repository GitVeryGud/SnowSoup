//
//  Transform.cpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 27/08/24.
//

#include "Transform.hpp"

simd_float4x4 Transform::RotationMatrix(bool pure){
    simd::float4x4 rotationMatrix;
    
    vector_float3 tempRotation = rotation * SNOWSOUP_PI_F/180;
    
    simd::float4x4 rotationX;
    simd::float4x4 rotationY;
    simd::float4x4 rotationZ;
    
    rotationX.columns[0] = { 1, 0, 0, 0 };
    rotationX.columns[1] = {0,(float)cos(tempRotation.x),-(float)sin(tempRotation.x), 0 };
    rotationX.columns[2] = {0, (float)sin(tempRotation.x),(float)cos(tempRotation.x), 0 };
    rotationX.columns[3] = {0, 0, 0, 1 };
    
    rotationY.columns[0] = { (float)cos(tempRotation.y), 0, (float)sin(tempRotation.y), 0 };
    rotationY.columns[1] = { 0, 1, 0, 0 };
    rotationY.columns[2] = {-(float)sin(tempRotation.y), 0,(float)cos(tempRotation.y), 0 };
    rotationY.columns[3] = { 0, 0, 0, 1 };
    
    rotationZ.columns[0] = { (float)cos(tempRotation.z), -(float)sin(tempRotation.z), 0, 0 };
    rotationZ.columns[1] = { (float)sin(tempRotation.z), (float)cos(tempRotation.z), 0, 0 };
    rotationZ.columns[2] = {0, 0, 1, 0 };
    rotationZ.columns[3] = { 0, 0, 0, 1 };
    
    if (pure) {
        rotationMatrix = rotationX;
        rotationMatrix *= rotationY;
        rotationMatrix *= rotationZ;
        return rotationMatrix;
    }
    
    rotationMatrix = rotationX;
    if (parent != NULL && parent->isTransform) {
        rotationMatrix = dynamic_cast<Transform*>(parent)->RotationMatrix(true);
        rotationMatrix *= rotationX;
    }
    rotationMatrix *= rotationY;
    rotationMatrix *= rotationZ;
    
    return rotationMatrix;
}

simd_float4x4 Transform::TransformMatrix(){
    simd::float4x4 transformMatrix;
    
    simd::float4x4 scaleMatrix;
    
    scaleMatrix = simd_matrix(
                              (simd_float4){scale.x, 0, 0, 0 },
                              (simd_float4){0, scale.y, 0, 0 },
                              (simd_float4){0, 0, scale.z, 0 },
                              (simd_float4){0, 0, 0, 1 }
                          );
    
    simd::float4x4 translationMatrix;

    translationMatrix = simd_matrix(
                                    (simd_float4){1, 0, 0, 0 },
                                    (simd_float4){0, 1, 0, 0 },
                                    (simd_float4){0, 0, 1, 0 },
                                    (simd_float4){position.x, position.y, position.z, 1 }
                                );
    
    transformMatrix = translationMatrix;
    if (parent != NULL && parent->isTransform) {
        transformMatrix = dynamic_cast<Transform*>(parent)->TransformMatrix();
        transformMatrix *= translationMatrix;
    }
        
    transformMatrix *= RotationMatrix(true);
    transformMatrix *= scaleMatrix;
    
    return transformMatrix;
}

vector_float3 Transform::Forward(){
    simd::float4 forward = vector4(0.f, 0.f, 1.f, 0.f);
    
    return simd_mul(forward, RotationMatrix(true)).xyz;
}

vector_float3 Transform::Right(){
    simd::float4 right = vector4(1.f, 0.f, 0.f, 0.f);
    
    return simd_mul(right, RotationMatrix(true)).xyz;
}
vector_float3 Transform::Up(){
    simd::float4 up = vector4(0.f, 1.f, 0.f, 0.f);
    
    return simd_mul(up, RotationMatrix(true)).xyz;
}
