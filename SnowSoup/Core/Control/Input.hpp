//
//  Input.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 26/08/24.
//

#ifndef Input_hpp
#define Input_hpp

#include "SnowInputs.h"

class Input {
public:    
    Input();
    
    bool pressedKeys[256];
    bool justPressedKeys[256];
    bool justReleasedKeys[256];
    
    bool isKeyPressed(unsigned short key);
    bool isKeyJustPressed(unsigned short key);
    bool isKeyJustReleased(unsigned short key);
};

#endif /* Input_h */
