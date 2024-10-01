//
//  BoxCollider.cpp
//  SnowSoup
//
//  Created by Gustavo Binder on 09/09/24.
//

#include "BoxCollider.hpp"

int BoxCollider::hasIntersected(BoxCollider* otherBox) {
    BoxCollider* col1 = this;
    BoxCollider* col2 = otherBox;
    
    if (col1->position.x + scale.x < otherBox->position.x - scale.x)
        return 0;
    if (col1->position.x - scale.x > otherBox->position.x + scale.x)
        return 0;
    
    if (col1->position.y + scale.y < otherBox->position.y - scale.y)
        return 0;
    if (col1->position.y - scale.y > otherBox->position.y + scale.y)
        return 0;
    
    if (col1->position.z + scale.z < otherBox->position.z - scale.z)
        return 0;
    if (col1->position.z - scale.z > otherBox->position.z + scale.z)
        return 0;
    
    return 1;
}
