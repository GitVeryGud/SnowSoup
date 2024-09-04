//
//  MetalView.mm
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 26/08/24.
//

#include "MetalView.hpp"

@implementation MetalView

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (void)setInputManager:(Input *)input {
    self->input = input;
}

- (void)interpretKeyEvents:(NSArray<NSEvent *> *)eventArray {
    for (int i = 0; i < eventArray.count; i++) {
        printf("%c\n", eventArray[i].keyCode);
    }
}

- (void)keyDown:(NSEvent *)event {
    int key;
    
    if (event.type == NSEventTypeKeyDown) {
        
        key = (int)event.keyCode;
        printf("%d\n", key);
        
        if (self->input->pressedKeys[key]) {
            if (self->input->justPressedKeys[key] == true)
                self->input->justPressedKeys[key] = false;
        } else {
            self->input->pressedKeys[key] = true;
            self->input->justPressedKeys[key] = true;
            self->input->justReleasedKeys[key] = false;
        }
    }
}

- (void)keyUp:(NSEvent *)event {
    int key;
    
    if (event.type == NSEventTypeKeyUp) {
        key = (int)event.keyCode;
        if (self->input->justReleasedKeys[key]) {
            self->input->justReleasedKeys[key] = false;
        } else {
            self->input->pressedKeys[key] = false;
            self->input->justPressedKeys[key] = false;
            self->input->justReleasedKeys[key] = true;
        }
    }
}

@end
