//
//  Collider.hpp
//  SnowSoup
//
//  Created by Gustavo Binder on 09/09/24.
//

#ifndef Collider_hpp
#define Collider_hpp

#include "Core/Transform/Transform.hpp"

class Collider : public Transform {
public:
    virtual int hasCollided();
    virtual int hasCollidedWith(Collider* collider);
    
    void setTag(std::string tag);
    std::string getTag();
    
    void setCollidersPtr(Collider** allColliders);
    
private:
    Collider** allColliders = nullptr;
    std::string tag;
};

#endif /* Collider_hpp */
