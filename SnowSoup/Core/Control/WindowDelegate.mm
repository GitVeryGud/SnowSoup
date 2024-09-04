//
//  WindowDelegate.mm
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 26/08/24.
//

#include "WindowDelegate.hpp"

@implementation WindowDelegate

- (void)windowDidBecomeKey:(NSNotification*)notification {
    //NSLog(@"Window: become key");
}

- (void)windowDidBecomeMain:(NSNotification*)notification {
    //NSLog(@"Window: become main");
}

- (void)windowDidResignKey:(NSNotification*)notification {
    //NSLog(@"Window: resign key");
}

- (void)windowDidResignMain:(NSNotification*)notification {
    //NSLog(@"Window: resign main");
}

// Runs while resizing
- (void)windowDidResize:(NSNotification *)notification {
    NSWindow* window = notification.object;
    float aspectRatio = window.frame.size.width / window.frame.size.height;
    renderer->setCameraAspect(aspectRatio);
    
    float TextureWidth = window.frame.size.width*2;
    float TextureHeight = window.frame.size.height*2;
    
    layer.drawableSize = CGSizeMake(TextureWidth, TextureHeight);
    CA::MetalDrawable* metalDrawable = (__bridge CA::MetalDrawable*)[layer nextDrawable];
    renderer->createDepthAndTargetTextures(TextureWidth, TextureHeight);
    renderer->draw(metalDrawable, sceneTree);
}

// Runs on end of resize
- (void)windowDidEndLiveResize:(NSNotification *)notification {
    NSWindow* window = notification.object;
    float aspectRatio = window.frame.size.width / window.frame.size.height;
    renderer->setCameraAspect(aspectRatio);
    
    float TextureWidth = window.frame.size.width*3;
    float TextureHeight = window.frame.size.height*3;
    
    layer.drawableSize = CGSizeMake(TextureWidth, TextureHeight);
    CA::MetalDrawable* metalDrawable = (__bridge CA::MetalDrawable*)[layer nextDrawable];
    renderer->createDepthAndTargetTextures(TextureWidth, TextureHeight);
    renderer->draw(metalDrawable, sceneTree);
}

- (void)windowWillClose:(NSNotification*)notification {
//    Window * window = notification.object;
    NSLog(@"Window: terminate");
    [NSApp terminate: NSApp];
}

- (void)setRenderer:(Renderer *)renderer{
    self->renderer = renderer;
}

- (void)setLayer:(CAMetalLayer *)layer {
    self->layer = layer;
}

- (void)setSceneTree:(Node *)sceneTree{
    self->sceneTree = sceneTree;
}

- (BOOL)acceptsFirstResponder { return YES; }
- (BOOL)canBecomeKeyWindow { return YES; }
- (BOOL)canBecomeMainWindow { return YES; }

@end
