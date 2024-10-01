//
//  Collider.cpp
//  SnowSoup
//
//  Created by Gustavo Binder on 09/09/24.
//

#include "Collider.hpp"

void Collider::setTag(std::string tag) {
    this->tag = tag;
}

std::string Collider::getTag() {
    return tag;
}

void Collider::setCollidersPtr(Collider** allColliders) {
    this->allColliders = allColliders;
}

int Collider::hasCollided() {
    return -1;
}

int Collider::hasCollidedWith(Collider* collider) {
    return -1;
}
