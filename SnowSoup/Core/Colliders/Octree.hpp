//
//  Octree.hpp
//  SnowSoup
//
//  Created by Gustavo Binder on 02/10/24.
//

#ifndef Octree_hpp
#define Octree_hpp

#include "Collider.hpp"
#include "Core/Model/Cube.hpp"

#define MAX_OC_SIZE 10

class OcNode {
public:
    OcNode(int subdiv, simd_float3 center, float size, Cube* cube) {
        memset(subOcs, 0, sizeof(OcNode*) * 8);
        this->subdiv = subdiv;
        this->center = center;
        this->size = size;
        this->halfSize = size/2;
        this->myCube = cube;
    };
    
    void updateOctree();
    void reinsert(Collider* col);
    
    void addCollider(Collider* col);
    void removeCollider(Collider* col);
    
    void subdivide();
    void sendColToSubOcs(Collider* col);
    
    bool isColInside(Collider* col);
    
    int subdiv, totalColliders;
    bool isDivided = false;
    
    simd_float3 center;
    float size, halfSize;
    
    std::vector<Collider*> myColliders;
    OcNode* subOcs[8];
    
    Cube* myCube;
};

//class Octree {
//public:
//    void buildTree(std::vector<Collider*>* allColliders, OcNode* currentOc, double subdiv, vector_float3 center);
//    void destroyTree();
//    
//private:
//    double bounding_box_size = 0.f;
//    OcNode* baseOc = nullptr;
//};

#endif /* Octree_hpp */
