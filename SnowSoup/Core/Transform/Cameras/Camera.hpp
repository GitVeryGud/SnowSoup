//
//  Camera.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 27/08/24.
//

#ifndef Camera_hpp
#define Camera_hpp

#include "Transform.hpp"
#include <simd/simd.h>

class Camera : public Transform{
    public:
    inline Camera(){};
    virtual ~Camera() = 0;
    float aspectRatio = 1.f;
    float near = 0.1f;
    float far = 100.f;
    
    // Used in Perspective and Orthographic cameras
    float fieldOfView = 60.f;
    float halfSize = 10.f;

    simd_float4x4 ViewMatrix();
    virtual simd_float4x4 ProjectionMatrix() = 0;
    simd_float4x4 TransformMatrix();
};

#endif /* Camera_hpp */
