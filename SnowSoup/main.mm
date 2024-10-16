#include "SnowSoup.hpp"
#include "Core/Players/MovingCollider.hpp"
#include "Core/Model/Cube.hpp"
#include "Core/Colliders/Collider.hpp"

#include <stdlib.h>

#define COLLIDER_AMOUNT 50

int main(int argc, const char * argv[]) {
    SnowSoup engine;
//    ModelTest* t;
//    Model* m;
    Cube* cube;
    
    MovingCollider *c[COLLIDER_AMOUNT];
    
    srand(0);
    
    engine.init();
    cube = new Cube(engine.device);
    cube->setColor(1.f, 0.f, 0.f);
    cube->buildBuffers();
    
//    c0 = new Collider(cube);
//    c1 = new Collider(cube);
//    c2 = new Collider(cube);
//    c3 = new Collider(cube);
//    
//    c0->position = simd_make_float3(1.f, 0.1f, 0.3f);
//    c1->position = simd_make_float3(-1.f, 0.f, 0.f);
//    c2->position = simd_make_float3(-0.6f, 3.f, -0.3f);
//    c3->position = simd_make_float3(0.2f, 2.f, 0.2f);
    
    for (int i = 0; i < COLLIDER_AMOUNT; i++) {
        c[i] = new MovingCollider(cube);
        c[i]->position =
        simd_make_float3((rand() % 1000) / 50 - 4.5f, (rand() % 1000) / 50 - 4.5f, (rand() % 1000) / 50 - 4.5f);
        c[i]->scale = simd_make_float3(rand() % 2 + 0.5f, rand() % 2 + 0.5f, rand() % 2 + 0.5f);
//        simd_make_float3(-2.f,-2.f,2.f);
        engine.addNode(c[i]);
    }
//    t = new ModelTest(engine.device);
//    m = new Model(engine.device);
//    t->input = engine.input;
//    
//    t->importModel("Models/Anime_charcter2.dae");
//    m->importModel("Models/Anime_charcter2.dae");
//    
//    t->AddChild(m);
//    t->myHead = t->baseNode->findMeshnode("Cabeza_001");
//    t->myHair = t->baseNode->findMeshnode("export_001");
//    
//    engine.addNode(t);
//    
//    t->position.z = 7.f;
//    t->position.y = -2.5f;
//    
//    m->position.z = 5.f;
//    m->rotation.y = 60.f;
//    m->rotation.x = 30.f;
    
//    engine.addNode(c0);
//    engine.addNode(c1);
//    engine.addNode(c2);
//    engine.addNode(c3);
    
    engine.run();
    
    return 0;
}
