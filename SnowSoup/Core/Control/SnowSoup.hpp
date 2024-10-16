//
//  SnowSoup.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 23/08/24.
//

#ifndef SnowSoup_hpp
#define SnowSoup_hpp

#include <AppKit/AppKit.h>
#include <Metal/Metal.hpp>
#include <chrono>
#include <thread>

#include <QuartzCore/CAMetalDrawable.hpp>

#include "WindowDelegate.hpp"
#include "MetalView.hpp"
#include "Input.hpp"
#include "Core/Nodes/Node.hpp"
#include "Renderer.hpp"
#include "Core/Players/ModelTest.hpp"
#include "Core/Colliders/Collider.hpp"
#include "Core/Colliders/Octree.hpp"

class SnowSoup{
public:
    void init();
    void run();
    
    void addNode(Node* newNode);
    
    MTL::Device* device;
    
    Input* input;
private:
    void initApp();
    void initDevice();
    
    void initWindow(NSSize windowSize, NSString* windowName);
    void initWindow(NSSize windowSize);
    void initWindow();
    
    Node* sceneTree = nullptr;
    
    NSWindow* metalWindow;

    CAMetalLayer* metalLayer;
    
    Renderer* renderer;
    
    std::vector<Collider*>* allColliders = nullptr;
    
    OcNode* baseOcNode = nullptr;
    float deltaTime;
};

#endif /* SnowSoup_hpp */
