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
    myCube->color = color;
    myCube->Draw(pEnc, uniforms, phongUniforms);
    setCubeColor(1.f, 0.f, 0.f);
}

void Collider::collidedCallback(Collider* other) {
    // Collided!!
}

void Collider::setCubeColor(float r, float g, float b) {
    color.x = r;
    color.y = g;
    color.z = b;
}

void support(const void *obj, const ccd_vec3_t *dir, ccd_vec3_t *vec)
{
    // assume that obj_t is user-defined structure that holds info about
    // object (in this case box: x, y, z, pos, quat - dimensions of box,
    // position and rotation)
    Collider *objl = (Collider *)obj;
    ccd_vec3_t dirl, pos;
    ccd_quat_t qinv, quat;
    
    Quaternion q = objl->quaternion();
    quat.q[0] = q.x;
    quat.q[1] = q.y;
    quat.q[2] = q.z;
    quat.q[3] = q.w;

    // apply rotation on direction vector
    ccdVec3Copy(&dirl, dir);
    ccdQuatInvert2(&qinv, &quat);
    ccdQuatRotVec(&dirl, &qinv);

    // compute support point in specified direction
    ccdVec3Set(vec, ccdSign(ccdVec3X(&dirl)) * objl->scale.x * CCD_REAL(0.5),
                ccdSign(ccdVec3Y(&dirl)) * objl->scale.y * CCD_REAL(0.5),
                ccdSign(ccdVec3Z(&dirl)) * objl->scale.z * CCD_REAL(0.5));

    // transform support point according to position and rotation of object
    ccdQuatRotVec(vec, &quat);
    
    pos.v[0] = objl->position.x;
    pos.v[1] = objl->position.y;
    pos.v[2] = objl->position.z;
    
    ccdVec3Add(vec, &pos);
}

