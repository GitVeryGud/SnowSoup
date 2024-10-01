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
    if (myHead == nullptr || myHair == nullptr) {
        printf("It's null\n");
        return;
    }
    
    if (input->isKeyJustPressed(KEY_Q)) {
//        printf("Yooo!\n");
        ctrlHead = !ctrlHead;
    }
    
    if (input->isKeyPressed(KEY_LEFT)) {
        if (ctrlHead) {
            myHead->rotation.z += 1.f;
            myHair->rotation.z += 1.f;
        } else
            rotation.y += 1.f;
    }
    if (input->isKeyPressed(KEY_RIGHT)) {
        if (ctrlHead) {
            myHead->rotation.z -= 1.f;
            myHair->rotation.z -= 1.f;
        } else
            rotation.y -= 1.f;
    }
}
