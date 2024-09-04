//
//  DirectionalLight.cpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 03/09/24.
//

#include "DirectionalLight.hpp"

simd_float4x4 DirectionalLight::RotationMatrix(){
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
    
    rotationMatrix = rotationX;
    rotationMatrix *= rotationY;
    rotationMatrix *= rotationZ;
    
    return rotationMatrix;
}

vector_float3 DirectionalLight::Direction() {
    vector_float4 baseDir = vector4(0.f, 0.f, -1.f, 1.f);
    simd_float4x4 rot = DirectionalLight::RotationMatrix();
    vector_float3 out = simd_mul(baseDir, rot).xyz;
    
    return out;
}

Camera* DirectionalLight::LightMapCamera(){
    OrthographicCamera* camera = new OrthographicCamera();
    camera->near = -500.f;
    camera->far = 500.f;
    camera->halfSize = 50.f;
    camera->rotation = rotation;
    return camera;
}
