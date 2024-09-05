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

bool Input::isMouseDown(unsigned short key) {
    return key > 0 ? (rightMouseState) : (leftMouseState);
}

void Input::resetMouseDelta() {
    mouseDelta.x = 0;
    mouseDelta.y = 0;
}

simd_float2 Input::currentMousePosition() {
    return mousePosition;
}

simd_float2 Input::getMouseDelta() {
    return mouseDelta;
}
