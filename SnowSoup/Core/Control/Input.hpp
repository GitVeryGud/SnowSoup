//
//  Input.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 26/08/24.
//

#ifndef Input_hpp
#define Input_hpp

#include <simd/simd.h>
#include "SnowInputs.h"

class Input {
public:    
    Input();
    
    simd_float2 mousePosition, mouseDelta;
    
    bool pressedKeys[256];
    bool justPressedKeys[256];
    bool justReleasedKeys[256];
    
    bool leftMouseState;
    bool rightMouseState;
    
    bool isKeyPressed(unsigned short key);
    bool isKeyJustPressed(unsigned short key);
    bool isKeyJustReleased(unsigned short key);
    bool isMouseDown(unsigned short key);
    
    void resetMouseDelta();
    
    simd_float2 currentMousePosition();
    simd_float2 getMouseDelta();
    
};

#endif /* Input_h */
