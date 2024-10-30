//
//  CollisionManager.hpp
//  SnowSoup
//
//  Created by Gustavo Binder on 29/10/24.
//

#ifndef CollisionManager_hpp
#define CollisionManager_hpp

#include <ccd/ccd.h>

//#include "Collider.hpp"
#include "Core/Colliders/Collider.hpp"

class CollisionManager {
public:
    static CollisionManager* getInstance();
    CollisionManager(CollisionManager &other) = delete;
    void operator=(const CollisionManager &) = delete;
    
    int hasBoxesCollided(Collider* col1, Collider* col2);
    void collideAllBoxes(std::vector<Collider*>* allColliders);
    
private:
    ccd_t ccd;
    friend class Collider;
    
protected:
    CollisionManager();
    static CollisionManager* instance;
};

#endif /* CollisionManager_hpp */
