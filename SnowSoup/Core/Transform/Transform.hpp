//
//  Transform.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 27/08/24.
//

#ifndef Transform_hpp
#define Transform_hpp

#include <simd/simd.h>
#include "SnowDefs.h"
#include "Core/Nodes/Node.hpp"

class Transform : public Node{
    public:
    Transform(){ isTransform = true; };
    vector_float3 position = vector3(0.f, 0.f, 0.f);
    vector_float3 rotation = vector3(0.f, 0.f, 0.f);
    vector_float3 scale = vector3(1.f, 1.f, 1.f);
    
    simd_float4x4 RotationMatrix(bool pure);
    simd_float4x4 TransformMatrix();
    void overrideTransformMatrixValue(int row, int col, float value);
    
    vector_float3 Forward();
    vector_float3 Right();
    vector_float3 Up();
};

#endif /* Transform_hpp */
