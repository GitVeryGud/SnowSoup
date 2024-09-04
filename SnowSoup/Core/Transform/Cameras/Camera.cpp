//
//  Camera.cpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 27/08/24.
//

#include "Camera.hpp"

Camera::~Camera() {};

simd_float4x4 Camera::ViewMatrix(){
    vector_float3 up = Up();
    vector_float3 target = position + Forward();
    
    vector_float3 zaxis = simd_normalize(target - position);
    vector_float3 xaxis = simd_normalize(simd_cross(zaxis, up));
    vector_float3 yaxis = simd_cross(xaxis, zaxis);
    
    simd::float4x4 M = simd_matrix(
                                   (simd_float4){xaxis.x, yaxis.x, -zaxis.x, 0 },
                                   (simd_float4){xaxis.y, yaxis.y, -zaxis.y, 0 },
                                   (simd_float4){xaxis.z, yaxis.z, -zaxis.z, 0 },
                                   (simd_float4){0, 0, 0, 1 }
                                   );
    
    simd::float4x4 translationMatrix = simd_matrix(
                                    (simd_float4){1, 0, 0, 0 },
                                    (simd_float4){0, 1, 0, 0 },
                                    (simd_float4){0, 0, 1, 0 },
                                    (simd_float4){-position.x, -position.y, -position.z, 1 }
                                );
    
    return M * translationMatrix;
}
