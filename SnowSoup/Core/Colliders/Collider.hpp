//
//  Collider.hpp
//  SnowSoup
//
//  Created by Gustavo Binder on 09/09/24.
//

#ifndef Collider_hpp
#define Collider_hpp

#include "Core/Transform/Transform.hpp"
#include "Core/Model/Cube.hpp"

class Collider : public Transform {
public:
    Collider(Cube* cube) {
        isCollider = true;
        isPrimitive = true;
        myCube = cube;
    };
    
    bool hasCollided();
    
    void setCollidersPtr(std::vector<Collider*>* allColliders);
    void Draw( MTL::RenderCommandEncoder* pEnc, Snow_Uniforms* uniforms, Snow_PhongUniforms* phongUniforms ) override;
    
    bool willCheckCollision = true;
    bool canCollideWithOthers = true;
    
    vector_float3 previousPosition = position;
    
private:
    std::vector<Collider*>* allColliders = nullptr;
    Cube* myCube;
};

#endif /* Collider_hpp */
