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
    
    Cube* cube = new Cube(device);
    cube->setColor(0.f, 1.f, 0.f);
    cube->buildBuffers();
    
//    baseOcNode = new OcNode(0, simd_make_float3(0), 20, cube);
    renderer = new Renderer(device, baseOcNode);
    allColliders = new std::vector<Collider*>();
    
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
    [metalWindow setAcceptsMouseMovedEvents:YES];
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
    
    Node* stack[256], *current;
    int sp = 0;
    
    stack[sp] = newNode;
    
    do {
        current = stack[sp];
        
        if (current->isCollider) {
            allColliders->push_back(dynamic_cast<Collider*>(current));
//            baseOcNode->addCollider(dynamic_cast<Collider*>(current));
        }
        
        sp--;
        
        for (int i = 0; i < current->childrenCount; i++) {
            sp++;
            stack[sp] = current->children[i];
        }
        
    } while (sp >= 0);
}

void SnowSoup::run() {
    NSEvent *event;
    int running = 1;
    
    std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point b = std::chrono::system_clock::now();
    
    while (running) {
        @autoreleasepool {
            a = std::chrono::system_clock::now();
            std::chrono::duration<double, std::milli> work_time = a - b;
            
            if (work_time.count() < 15.0) {
                std::chrono::duration<double, std::milli> delta_ms(15.0 - work_time.count());
                auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
                std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
            }
            
            b = std::chrono::system_clock::now();
            std::chrono::duration<double, std::milli> sleep_time = b - a;
            deltaTime = sleep_time.count();
//            printf("%f\n", deltaTime);
            
            input->resetMouseDelta();
            
            while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES])) {

                [NSApp sendEvent:event];
                [NSApp updateWindows];
            }
            
            renderer->camera->rotation.y -= 1 * input->getMouseDelta().x;
            renderer->camera->rotation.x += 1 * input->getMouseDelta().y;
            
            vector_float3 off_r = renderer->camera->Right();
            vector_float3 off_f = renderer->camera->Forward();
            
            if(input->isKeyPressed(KEY_W)) {
                renderer->camera->position.z += 1 * off_f.z * deltaTime;
                renderer->camera->position.x += 1 * off_f.x * deltaTime;
            }
            if(input->isKeyPressed(KEY_S)) {
                renderer->camera->position.z -= 1 * off_f.z * deltaTime;
                renderer->camera->position.x -= 1 * off_f.x * deltaTime;
            }
            if(input->isKeyPressed(KEY_A)) {
                renderer->camera->position.z += 1 * off_r.z * deltaTime;
                renderer->camera->position.x += 1 * off_r.x * deltaTime;
            }
            if(input->isKeyPressed(KEY_D)) {
                renderer->camera->position.z -= 1 * off_r.z * deltaTime;
                renderer->camera->position.x -= 1 * off_r.x * deltaTime;
            }
            
            if(input->isKeyPressed(KEY_O)){renderer->angle += 2.f;}
            if(input->isKeyPressed(KEY_P)){renderer->angle -= 2.f;}
            
            CA::MetalDrawable* metalDrawable = (__bridge CA::MetalDrawable*)[metalLayer nextDrawable];
            
            renderer->drawSetup(metalDrawable);
            renderer->draw(metalDrawable, sceneTree);
            renderer->endDraw(metalDrawable);
        }
    }
}

