//
//  MovingCollider.cpp
//  SnowSoup
//
//  Created by Gustavo Binder on 07/10/24.
//

#include "MovingCollider.hpp"

void MovingCollider::Update() {
//    position.x += velocity.x;
//    if (position.x > 10) {
//        position.x = 10;
//        velocity.x = -velocity.x;
//    }
//    if (position.x < -10) {
//        position.x = -10;
//        velocity.x = -velocity.x;
//    }
//    
//    position.y += velocity.y;
//    if (position.y > 10) {
//        position.y = 10;
//        velocity.y = -velocity.y;
//    }
//    if (position.y < -10) {
//        position.y = -10;
//        velocity.y = -velocity.y;
//    }
//    
//    position.z += velocity.z;
//    if (position.z > 10) {
//        position.z = 10;
//        velocity.z = -velocity.z;
//    }
//    if (position.z < -10) {
//        position.z = -10;
//        velocity.z = -velocity.z;
//    }
    double speed = 0.1f;
    
    if (input->pressedKeys[KEY_LEFT])
        position.x += speed;
    
    if (input->pressedKeys[KEY_RIGHT])
        position.x += -speed;
    
    if (input->pressedKeys[KEY_UP])
        position.y += speed;
    
    if (input->pressedKeys[KEY_DOWN])
        position.y += -speed;
    
    if (input->pressedKeys[KEY_U])
        rotation.y += speed * 2;
    
    if (input->pressedKeys[KEY_J])
        rotation.y += -speed * 2;
    
//    setCubeColor(1.f, 0.f, 0.f);
}


void MovingCollider::collidedCallback(Collider* other) {
    setCubeColor(0.f, 0.f, 1.f);
    printf("Collided\n");
}
