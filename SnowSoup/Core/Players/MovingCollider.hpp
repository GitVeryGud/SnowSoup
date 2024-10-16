//
//  MovingCollider.hpp
//  SnowSoup
//
//  Created by Gustavo Binder on 07/10/24.
//

#ifndef MovingCollider_hpp
#define MovingCollider_hpp

#include "Core/Colliders/Collider.hpp"

class MovingCollider : public Collider {
public:
    MovingCollider(Cube* cube) : Collider(cube) {
        velocity.x = (float)(rand() % 10 - 5) / 100;
        velocity.y = (float)(rand() % 10 - 5) / 100;
        velocity.z = (float)(rand() % 10 - 5) / 100;
    };
    
    vector_float3 velocity;
    
    void Update() override;
};

#endif /* MovingCollider_hpp */
