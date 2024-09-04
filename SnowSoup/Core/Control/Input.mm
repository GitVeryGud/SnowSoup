//
//  Input.mm
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 26/08/24.
//

#include "Input.hpp"

Input::Input() {
    for (int i = 0; i < 256; i++) {
        pressedKeys[i] = false;
        justPressedKeys[i] = false;
        justReleasedKeys[i] = false;
    }
}

bool Input::isKeyPressed(unsigned short key) {
    return pressedKeys[key];
}

bool Input::isKeyJustPressed(unsigned short key) {
    return justPressedKeys[key];
}

bool Input::isKeyJustReleased(unsigned short key) {
    return justReleasedKeys[key];
}
