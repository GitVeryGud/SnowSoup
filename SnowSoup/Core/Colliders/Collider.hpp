//
//  Collider.hpp
//  SnowSoup
//
//  Created by Gustavo Binder on 09/09/24.
//

#ifndef Collider_hpp
#define Collider_hpp

#include <ccd/ccd.h>
#include <ccd/quat.h>

#include "Core/Transform/Transform.hpp"
#include "Core/Model/Cube.hpp"
//#include "CollisionManager.hpp"

class Collider : public Transform {
public:
    Collider(Cube* cube) {
        isCollider = true;
        isPrimitive = true;
        myCube = cube;
        setCubeColor(1.f, 0.f, 0.f);
        scale = vector3(2.f,2.f,2.f);
    };
    
    simd_float3 color;
    
    virtual void collidedCallback(Collider* other);
    
    void setCollidersPtr(std::vector<Collider*>* allColliders);
    void Draw( MTL::RenderCommandEncoder* pEnc, Snow_Uniforms* uniforms, Snow_PhongUniforms* phongUniforms ) override;
    void setCubeColor(float r, float g, float b);
    
private:
    std::vector<Collider*>* allColliders = nullptr;
    Cube* myCube;
    
    friend class CollisionManager;
};

void support(const void *obj, const ccd_vec3_t *dir, ccd_vec3_t *vec);

#endif /* Collider_hpp */
