#include "SnowSoup.hpp"
#include "Core/Players/ModelTest.hpp"

int main(int argc, const char * argv[]) {
    SnowSoup engine;
    ModelTest* t;
    Model* m;
    
    engine.init();
    
    t = new ModelTest(engine.device);
    m = new Model(engine.device);
    t->input = engine.input;
    
    t->importModel("Models/Anime_charcter2.dae");
    m->importModel("Models/Anime_charcter2.dae");
    
    t->AddChild(m);
    t->myHead = dynamic_cast<MeshNode*>(t->baseNode->children[0]);
    engine.addNode(t);
    
    t->position.z = 7.f;
    t->position.y = -2.5f;
    
    m->position.z = 5.f;
    m->rotation.y = 60.f;
    m->rotation.x = 30.f;
    
    engine.run();
    
    return 0;
}
