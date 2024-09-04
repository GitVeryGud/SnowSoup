//
//  Mesh.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 27/08/24.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <Metal/Metal.hpp>
#include <simd/simd.h>

class Mesh {
public:
    MTL::Buffer     *indexBuffer    = nullptr;
    UInt32          *indices        = nullptr;
    size_t          indexAmount     = 0;
};


#endif /* Mesh_hpp */
