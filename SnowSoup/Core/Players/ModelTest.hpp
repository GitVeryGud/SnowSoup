//
//  ModelTest.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 06/09/24.
//

#ifndef ModelTest_hpp
#define ModelTest_hpp

#include <Metal/Metal.hpp>

#include "model.hpp"
#include "Input.hpp"

class ModelTest : public Model {
public:
    ModelTest(MTL::Device* device);
    
    Input* input;
    void Update() override;
};


#endif /* ModelTest_hpp */
