//
//  BoxCollider.hpp
//  SnowSoup
//
//  Created by Gustavo Binder on 09/09/24.
//

#ifndef BoxCollider_hpp
#define BoxCollider_hpp

#include "Core/Colliders/Collider.hpp"

class BoxCollider : public Collider {
public:
//    BoxCollider();
//    simd::float3 vertices[8];
//    int indices[36];
    
    int hasIntersected(BoxCollider* otherBox);
};

#endif /* BoxCollider_hpp */
