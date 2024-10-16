//
//  CollisionManager.cpp
//  SnowSoup
//
//  Created by Gustavo Binder on 15/10/24.
//

#include "CollisionManager.hpp"

void CollisionManager::checkCollisions(std::vector<Collider*> colliders) {
    for (int i = 0; i < colliders.size(); ++i) {
        for (int j = i + 1; j < colliders.size(); ++j) {
            
        }
    }
}

simd_int2 CollisionManager::minmaxPointsAlongAxis(Collider* collider, simd_float3 axis) {
    simd_int2 minmax;
    int allProjections[8];
    float s = 0.5f;
    
    simd_float4 vertices[8] = {
        {s, s, s, 1},
        {-s, s, s, 1},
        {-s, s, -s, 1},
        {s, s, -s, 1},
        {s, -s, s, 1},
        {-s, -s, s, 1},
        {-s, -s, -s, 1},
        {s, -s, -s, 1},
    };
    
    for (int i = 0; i < 8; ++i) {
        simd_float4x4 rot = collider->TransformMatrix();
        for (int j = 0; j < 4; ++j) {
            float r = 0;
            r += vertices[i][0] * rot.columns[j][0];
            r += vertices[i][1] * rot.columns[j][1];
            r += vertices[i][2] * rot.columns[j][2];
            r += vertices[i][3] * rot.columns[j][3];
            
            vertices[i][j] = r;
        }
    }
    
    minmax[0] = axis[0] * vertices[0][0] + axis[1] * vertices[0][1] + axis[2] * vertices[0][2];
    minmax[1] = axis[0] * vertices[1][0] + axis[1] * vertices[1][1] + axis[2] * vertices[1][2];
    
    for (int i = 0; i < 8; ++i) {
        float x = axis[0] * vertices[i][0] + axis[1] * vertices[i][1] + axis[2] * vertices[i][2];
        
        if (x < minmax[0]) minmax[0] = x;
        if (x > minmax[1]) minmax[1] = x;
    }
    
    return minmax;
}

//simd_float3 crossProduct(simd_float3 v1, simd_float3 v2) {
//    
//}
