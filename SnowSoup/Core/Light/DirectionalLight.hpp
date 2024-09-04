//
//  DirectionalLight.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 03/09/24.
//

#ifndef DirectionalLight_hpp
#define DirectionalLight_hpp

#include <stdio.h>
#include <simd/simd.h>
#include "SnowDefs.h"
#include "../Transform/Cameras/OrthographicCamera.hpp"
#include "../Transform/Cameras/Camera.hpp"


class DirectionalLight{
    public:
    inline DirectionalLight(){};
    vector_float3 rotation = vector3(0.f, 0.f, 0.f);
    vector_float3 color;
    vector_float3 Direction();
    
    simd_float4x4 RotationMatrix();
    Camera* LightMapCamera();
};


#endif /* DirectionalLight_hpp */
