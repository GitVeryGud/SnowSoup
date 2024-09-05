//
//  Node.cpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 03/09/24.
//

#include "Node.hpp"

Node::Node() {
    parent = nullptr;
    childrenCount = 0;
}

void Node::AddChild(Node* child) {
    children[childrenCount] = child;
    child->parent = this;
    childrenCount++;
}

void Node::Draw( MTL::RenderCommandEncoder* pEnc, Snow_Uniforms* uniforms, Snow_PhongUniforms* phongUniforms) {

}

void Node::Update() {

}
