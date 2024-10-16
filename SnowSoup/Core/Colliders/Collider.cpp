//
//  Collider.cpp
//  SnowSoup
//
//  Created by Gustavo Binder on 09/09/24.
//

#include "Collider.hpp"

void Collider::Draw( MTL::RenderCommandEncoder* pEnc, Snow_Uniforms* uniforms, Snow_PhongUniforms* phongUniforms ) {
    myCube->scale = scale;
    myCube->rotation = rotation;
    myCube->position = position;
    myCube->Draw(pEnc, uniforms, phongUniforms);
}
