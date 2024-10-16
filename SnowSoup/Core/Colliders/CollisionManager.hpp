//
//  CollisionManager.hpp
//  SnowSoup
//
//  Created by Gustavo Binder on 15/10/24.
//

#ifndef CollisionManager_hpp
#define CollisionManager_hpp

#include "Core/Colliders/Collider.hpp"

class CollisionManager {
public:
    void checkCollisions(std::vector<Collider*> colliders);
    simd_int2 minmaxPointsAlongAxis(Collider* collider, simd_float3 axis);
    simd_float3 crossProduct(simd_float3 v1, simd_float3 v2);
    
    friend class Collider;
};

#endif /* CollisionManager_hpp */
