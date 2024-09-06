//
//  ModelTest.cpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 06/09/24.
//

#include "ModelTest.hpp"

ModelTest::ModelTest(MTL::Device *device) : Model(device) {
    importTexture("Models/textures.png");
}

void ModelTest::Update() {
    if (input->isKeyPressed(KEY_LEFT))
        rotation.y += 1.f;
    if (input->isKeyPressed(KEY_RIGHT))
        rotation.y -= 1.f;
}
