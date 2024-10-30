//
//  MovingCollider.hpp
//  SnowSoup
//
//  Created by Gustavo Binder on 07/10/24.
//

#ifndef MovingCollider_hpp
#define MovingCollider_hpp

#include "Core/Colliders/Collider.hpp"
#include "Core/Control/Input.hpp"

class MovingCollider : public Collider {
public:
    MovingCollider(Cube* cube, Input* input) : Collider(cube) {
        velocity.x = (float)(rand() % 10 - 5) / 100;
        velocity.y = (float)(rand() % 10 - 5) / 100;
        velocity.z = (float)(rand() % 10 - 5) / 100;
        
        setCubeColor(1.f, 0.f, 0.f);
        this->input = input;
        
        scale.x = 5.f;
    };
    
    vector_float3 velocity;
    Input* input;
    
    void Update() override;
    void collidedCallback(Collider* other) override;
};

#endif /* MovingCollider_hpp */
