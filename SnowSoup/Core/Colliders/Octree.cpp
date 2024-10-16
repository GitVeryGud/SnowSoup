//
//  Octree.cpp
//  SnowSoup
//
//  Created by Gustavo Binder on 02/10/24.
//

#include "Octree.hpp"
 
void OcNode::addCollider(Collider* col) {
//    for (int i = 0; i < myColliders.size(); i++) {
//        if (col != myColliders[i])
//            continue;
//        
//        if (!isColInside(col)) {
//            myColliders.erase(myColliders.begin() + i);
//            myColliders.shrink_to_fit();
//            --totalColliders;
//        }
//        
//        return;
//    }
    
    if (!isColInside(col))
        return;
    
    ++totalColliders;
    
    if (isDivided) {
//        totalColliders = 0;
        sendColToSubOcs(col);
        return;
    }
    
    myColliders.push_back(col); // Add collider to octant
    
    if (myColliders.size() > 3 && subdiv < 3) { // When collider amount is above 5, subdivide
        subdivide(); // Create the new octants and split the colliders between them
    }
}

void OcNode::reinsert(Collider* col) {
    
}

void OcNode::removeCollider(Collider* col) {
    
}

void OcNode::subdivide() {
    // Create all suboctants
    float quartSize = halfSize/2;
    subOcs[0] = new OcNode(subdiv + 1, simd_make_float3(center.x - quartSize, center.y - quartSize, center.z - quartSize), halfSize, myCube);
    subOcs[1] = new OcNode(subdiv + 1, simd_make_float3(center.x + quartSize, center.y - quartSize, center.z - quartSize), halfSize, myCube);
    subOcs[2] = new OcNode(subdiv + 1, simd_make_float3(center.x + quartSize, center.y + quartSize, center.z - quartSize), halfSize, myCube);
    subOcs[3] = new OcNode(subdiv + 1, simd_make_float3(center.x + quartSize, center.y + quartSize, center.z + quartSize), halfSize, myCube);
    subOcs[4] = new OcNode(subdiv + 1, simd_make_float3(center.x - quartSize, center.y + quartSize, center.z - quartSize), halfSize, myCube);
    subOcs[5] = new OcNode(subdiv + 1, simd_make_float3(center.x - quartSize, center.y + quartSize, center.z + quartSize), halfSize, myCube);
    subOcs[6] = new OcNode(subdiv + 1, simd_make_float3(center.x - quartSize, center.y - quartSize, center.z + quartSize), halfSize, myCube);
    subOcs[7] = new OcNode(subdiv + 1, simd_make_float3(center.x + quartSize, center.y - quartSize, center.z + quartSize), halfSize, myCube);
    
    totalColliders = 0;
    
    // Move colliders to new octants
    for (int i = 0; i < myColliders.size(); i++)
        sendColToSubOcs(myColliders[i]);
    
    // Clear current colliders
    myColliders.clear();
    
    // Set divided flag
    isDivided = true;
    
//    if (totalColliders > 10)
//        return;
//    
////    if (!isDivided)
////        return;
//    
//    for (int i = 0; i < 8; i++) {
//        for (int j = 0; j < subOcs[i]->myColliders.size(); j++) {
//            myColliders.push_back(subOcs[i]->myColliders[j]);
//        }
//        
//        subOcs[i]->myColliders.clear();
//        delete subOcs[i];
//        subOcs[i] = nullptr;
//    }
//    
//    isDivided = false;
}

void OcNode::sendColToSubOcs(Collider* col) {
    for (int i = 0; i < 8; i++) {
        subOcs[i]->addCollider(col);
        totalColliders += subOcs[i]->totalColliders;
    }
}

bool OcNode::isColInside(Collider* col) {
    float xScale, yScale, zScale;
    
    xScale = col->scale.x/2;
    yScale = col->scale.y/2;
    zScale = col->scale.z/2;
    
    if ((col->position.x + xScale <= center.x - halfSize ||
          col->position.x - xScale >= center.x + halfSize))
        return false;
    
    if ((col->position.y + yScale <= center.y - halfSize ||
          col->position.y - yScale >= center.y + halfSize))
        return false;
    
    if ((col->position.z + zScale <= center.z - halfSize ||
          col->position.z - zScale >= center.z + halfSize))
        return false;
    
    
    return true;
}

void OcNode::updateOctree() {
    OcNode* stack[256], *current;
    int sp = 0;
    
    stack[sp] = this;
    
    do {
        current = stack[sp];
        --sp;
        
        for (int i = 0; i < current->myColliders.size(); i++) {
            Collider* col = current->myColliders[i];
            if (col->previousPosition.x == col->position.x &&
                col->previousPosition.y == col->position.y &&
                col->previousPosition.z == col->position.z)
                continue;
            
            current->addCollider(col);
        }
        
        for (int i = 0; i < 8; i++) {
            if (current->subOcs[i] == nullptr)
                continue;
            ++sp;
            stack[sp] = current->subOcs[i];
        }
        
    } while (sp >= 0);
}
