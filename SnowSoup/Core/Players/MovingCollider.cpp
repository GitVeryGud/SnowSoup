//
//  MovingCollider.cpp
//  SnowSoup
//
//  Created by Gustavo Binder on 07/10/24.
//

#include "MovingCollider.hpp"

void MovingCollider::Update() {
//    printf("Hello!\n");
    
//    printf("%f %f %f\n", velocity.x, velocity.y, velocity.z);
    
//    return;
    position.x += velocity.x;
    if (position.x > 10) {
        position.x = 10;
        velocity.x = -velocity.x;
    }
    if (position.x < -10) {
        position.x = -10;
        velocity.x = -velocity.x;
    }
    
    position.y += velocity.y;
    if (position.y > 10) {
        position.y = 10;
        velocity.y = -velocity.y;
    }
    if (position.y < -10) {
        position.y = -10;
        velocity.y = -velocity.y;
    }
    
    position.z += velocity.z;
    if (position.z > 10) {
        position.z = 10;
        velocity.z = -velocity.z;
    }
    if (position.z < -10) {
        position.z = -10;
        velocity.z = -velocity.z;
    }
}
