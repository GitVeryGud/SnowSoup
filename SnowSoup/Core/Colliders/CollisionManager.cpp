//
//  CollisionManager.cpp
//  SnowSoup
//
//  Created by Gustavo Binder on 29/10/24.
//

#include "CollisionManager.hpp"

CollisionManager::CollisionManager() {
    CCD_INIT(&ccd);
    
    ccd.support1 = support;
    ccd.support2 = support;
    
    ccd.max_iterations = 100;
}

CollisionManager* CollisionManager::getInstance() {
    if (instance == nullptr)
        instance = new CollisionManager();
    
    return instance;
}

int CollisionManager::hasBoxesCollided(Collider* col1, Collider* col2) {
    return ccdGJKIntersect(col1, col2, &ccd);
}

void CollisionManager::collideAllBoxes(std::vector<Collider*>* allColliders) {
    for (int i = 0; i < allColliders->size(); ++i) {
        for (int j = i + 1; j < allColliders->size(); ++j) {
            if (hasBoxesCollided((*allColliders)[i], (*allColliders)[j])) {
                (*allColliders)[i]->collidedCallback((*allColliders)[j]);
                (*allColliders)[j]->collidedCallback((*allColliders)[i]);
            }
        }
//        printf("%d\n", i);
    }
}

CollisionManager *CollisionManager::instance = new CollisionManager();


