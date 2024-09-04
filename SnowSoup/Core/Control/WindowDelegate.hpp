//
//  WindowDelegate.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 26/08/24.
//

#ifndef WindowDelegate_hpp
#define WindowDelegate_hpp

#include <QuartzCore/CAMetalLayer.h>
#include <QuartzCore/CAMetalDrawable.hpp>
#include <AppKit/Appkit.h>
#include "Renderer.hpp"
#include "Node.hpp"

@interface WindowDelegate : NSObject<NSWindowDelegate>{
    Renderer* renderer;
    CAMetalLayer* layer;
    Node* sceneTree;
}

- (void)setRenderer : (Renderer*)renderer;
- (void)setLayer : (CAMetalLayer*)layer;
- (void)setSceneTree : (Node*)sceneTree;
@end

#endif /* WindowDelegate_hpp */
