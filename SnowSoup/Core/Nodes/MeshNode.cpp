//
//  MeshNode.cpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 03/09/24.
//

#include "MeshNode.hpp"

void MeshNode::extractRotation(){
    simd::float4x4 rotationMatrix = transformation;
    double syn;
    bool singular = false;
    
    // Zeroes position element
    this->position = vector3(transformation.columns[3][0], transformation.columns[3][1], transformation.columns[3][2]);
    rotationMatrix.columns[3] = { 0, 0, 0, 1 };
    
    // Extracts scale element
    float sx = simd_length(rotationMatrix.columns[0].xyz);
    float sy = simd_length(rotationMatrix.columns[1].xyz);
    float sz = simd_length(rotationMatrix.columns[2].xyz);
    this->scale = vector3(sx, sy, sz);
    
    // Divides matrix by scale resulting in rotation matrix
    rotationMatrix.columns[0] /= sx;
    rotationMatrix.columns[1] /= sy;
    rotationMatrix.columns[2] /= sz;
    
    this->rotationMatrix = rotationMatrix;
    double r21 = rotationMatrix.columns[1][2];
    double r22 = rotationMatrix.columns[2][2];
    double r20 = rotationMatrix.columns[0][2];
    double r10 = rotationMatrix.columns[0][1];
    double r11 = rotationMatrix.columns[1][1];
    double r12 = rotationMatrix.columns[2][1];
    double r00 = rotationMatrix.columns[0][0];
    
    syn = sqrt(pow(r21, 2) + pow(r22, 2));
    singular = syn < 1e-6;
    
    if (singular) {
        this->rotation.x = atan2(-r12, r11);
        this->rotation.y = atan2(-r20, syn);
        this->rotation.z = 0;
//        printf("Damn\n");
    } else {
        this->rotation.x = atan2(r21, r22);
        this->rotation.y = atan2(-r20, syn);
        this->rotation.z = atan2(r10, r00);
    }
    
    this->rotation.x = -(this->rotation.x * 180) / SNOWSOUP_PI_F;
    this->rotation.y = (this->rotation.y * 180) / SNOWSOUP_PI_F;
    this->rotation.z = (this->rotation.z * 180) / SNOWSOUP_PI_F;
}

MeshNode* MeshNode::findMeshnode(const char* name) {
    MeshNode* stack[1024], * current;
    int sp = 0;
    
    stack[sp] = this;
    
    do {
        current = stack[sp];
        
        printf("%s %s\n", name, current->name);
        
        if (strcmp(name, current->name) == 0) {
            printf("Found it!\n");
            break;
        }
        
        sp--;
        
        for (int i = 0; i < current->childrenCount; i++) {
            sp++;
            stack[sp] = dynamic_cast<MeshNode*>(current->children[i]);
        }
        
    } while (sp > 0);
    
    if (sp < 0)
        return nullptr;
        
    return stack[sp];
}
