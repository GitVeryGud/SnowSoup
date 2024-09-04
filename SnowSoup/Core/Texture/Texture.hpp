//
//  Texture.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 27/08/24.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <Metal/Metal.hpp>
#include <stb_image.h>

class Texture {
public:
    Texture (MTL::Device* device);
    ~Texture();
    void importTexture(const char* filepath);
    
    MTL::Texture* texture;
    int width, height, channels;
private:
    MTL::Device* device;
};

#endif /* Texture_hpp */
