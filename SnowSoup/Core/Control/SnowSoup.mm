//
//  SnowSoup.mm
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 23/08/24.
//

#include "SnowSoup.hpp"

void SnowSoup::init() {
    input = new Input;
    sceneTree = new Node;
    
    initApp();
    initDevice();
    
    renderer = new Renderer(device);
    
    initWindow();
    
    
}

void SnowSoup::initApp() {
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp setPresentationOptions:NSApplicationPresentationDefault];
    [NSApp activateIgnoringOtherApps:YES];
    
    [NSApp finishLaunching];
}

void SnowSoup::initDevice() {
    device = MTL::CreateSystemDefaultDevice();
}

void SnowSoup::initWindow(NSSize windowSize, NSString* windowName) {
    NSRect frame = NSMakeRect(100, 100, windowSize.width, windowSize.height);
    metalLayer = [CAMetalLayer layer];
    
    WindowDelegate* windowDelegate = [[WindowDelegate alloc] init];
    
    MetalView* metalView = [[MetalView alloc] init];
    [metalView setInputManager:input];
    metalView.wantsLayer = YES;
    metalView.layer = metalLayer;
    
    metalWindow = [[NSWindow alloc]
                   initWithContentRect:frame
                   styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable
                   backing:NSBackingStoreBuffered
                   defer:NO];
    [metalWindow setBackgroundColor:[NSColor blueColor]];
    [metalWindow makeKeyAndOrderFront: NSApp];
    metalWindow.contentView = metalView;
    metalWindow.delegate = windowDelegate;
    [metalWindow setTitle: windowName];
    
    
    metalLayer.device = (__bridge id<MTLDevice>)device;
    metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    metalLayer.allowsEdgeAntialiasing = true;
    metalLayer.drawableSize = CGSizeMake(512 * 3, 512 * 3);
    
    [windowDelegate setRenderer:renderer];
    [windowDelegate setSceneTree:sceneTree];
    [windowDelegate setLayer:metalLayer];
    
    
    printf("Window initialized\n");
}

void SnowSoup::initWindow(NSSize windowSize){
    initWindow(windowSize, @"Window");
}

void SnowSoup::initWindow(){
    initWindow(NSMakeSize(512, 512), @"Window");
}

void SnowSoup::addNode(Node* newNode) {
    sceneTree->AddChild(newNode);
}

void SnowSoup::run() {
    NSEvent *event;
    int running = 1;
    
    while (running) {
        @autoreleasepool {
            while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES])) {

                [NSApp sendEvent:event];
                [NSApp updateWindows];
            }
            
            CA::MetalDrawable* metalDrawable = (__bridge CA::MetalDrawable*)[metalLayer nextDrawable];
            
            renderer->drawSetup(metalDrawable);
            renderer->draw(metalDrawable, sceneTree);
            renderer->endDraw(metalDrawable);
        }
    }
}

