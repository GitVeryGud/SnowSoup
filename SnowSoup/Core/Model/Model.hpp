//
//  Model.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 27/08/24.
//

#ifndef Model_hpp
#define Model_hpp

#include <Metal/Metal.hpp>
#include <simd/simd.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "SnowStructs.h"
#include "Core/Texture/Texture.hpp"
#include "Core/Transform/Transform.hpp"
#include "Core/Nodes/MeshNode.hpp"

class Model : public Transform {
public:
    Model(MTL::Device* device);
    void importModel(const std::string filename);
    void importTexture(const char* filepath);
    void render(MTL::RenderCommandEncoder* pEnc, Snow_Uniforms* uniforms, Snow_PhongUniforms* phongUniforms);
    
    void Draw( MTL::RenderCommandEncoder* pEnc, Snow_Uniforms* uniforms, Snow_PhongUniforms* phongUniforms ) override;
    MeshNode* baseNode;
private:
    MTL::Device* device;
    MTL::Buffer* vertexBuffer;
    MTL::Buffer* textureBuffer;
    MTL::Buffer* normalsBuffer;
    
    Mesh** meshes;
//    MeshNode* baseNode;
    simd::float2* textureVertices = nullptr;
    simd::float3* vertices = nullptr;
    simd::float3* normals = nullptr;
    
    size_t numMeshes = 0;
    size_t numVertices = 0;
    size_t numTextVertices = 0;
    
    size_t totalVertexAmount = 0;
    
    Texture* texture = nullptr;
    
    void readModelNodeTree(MeshNode* node, aiNode* assimpNode, const aiScene* scene, aiMatrix4x4 carriedTransform);
    void setMesh(Mesh* mesh, aiMesh* assimpMesh, const aiScene* scene);
    void setMeshNodeTransformation(aiMatrix4x4 transform, MeshNode* node);
    void getVertexAmount(const aiScene* scene);
    
    void buildBuffers();
};

#endif /* Model_hpp */
