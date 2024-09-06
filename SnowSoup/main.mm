//
//  main.mm
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 23/08/24.
//

//#include "Core/Control/SnowSoup.hpp"
#include "SnowSoup.hpp"

int main(int argc, const char * argv[]) {
    SnowSoup engine;
    Model* boneco2;
    ModelTest* boneco;
    engine.init();
    
    boneco = new ModelTest(engine.device);
    boneco2 = new Model(engine.device);
    
    boneco->input = engine.input;
    
    boneco->position = vector3(-2.f, -3.f, 10.f);
    boneco2->position = vector3(2.f, -3.f, 4.f);
    
    boneco->importModel("Models/Anime_charcter2.dae");
    boneco2->importModel("Models/baoxiaoling2.obj");

    boneco2->importTexture("Models/zombie.png");
    
    //engine.addNode(boneco2);
    engine.addNode(boneco);
    
    engine.run();
    
    return 0;
}
