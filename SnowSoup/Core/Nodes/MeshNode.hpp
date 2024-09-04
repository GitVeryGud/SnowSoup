//
//  MeshNode.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 03/09/24.
//

#ifndef MeshNode_hpp
#define MeshNode_hpp

#include "Core/Model/Mesh.hpp"
#include "Core/Transform/Transform.hpp"

class MeshNode : public Transform {
public:
    Mesh** meshes = nullptr;
    matrix_float4x4 transformation, localTransformation;
    int meshCount = 0;
};

#endif /* MeshNode_hpp */
