//
//  Node.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 03/09/24.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <Metal/Metal.hpp>

#include "SnowStructs.h"

class Node {
public:
    Node* parent;
    Node* children[256];
    int childrenCount;
    
    Node();
    
    void AddChild(Node* child);
    virtual void Draw( MTL::RenderCommandEncoder* pEnc, Snow_Uniforms* uniforms, Snow_PhongUniforms* phongUniforms );
    virtual void Update();
};

#endif /* Node_hpp */
