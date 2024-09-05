//
//  Model.cpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 27/08/24.
//

#include "Model.hpp"

Model::Model(MTL::Device* device) {
    this->device = device;
    
    texture = new Texture(device);
    texture->importTexture("Models/textures.png");
}

void Model::importTexture(const char* filepath) {
    texture->importTexture(filepath);
}

void Model::importModel(const std::string filename) {
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(
                                             filename,
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate);
    assert(scene != nullptr);
    
    MeshNode* baseNode = new MeshNode;
    
    getVertexAmount(scene);
    
    if (vertices != nullptr)
        delete vertices;
    vertices = new simd::float3[totalVertexAmount];

    if (textureVertices != nullptr)
        delete textureVertices;
    textureVertices = new simd::float2[totalVertexAmount];
    
    if (normals != nullptr)
        delete normals;
    normals = new simd::float3[totalVertexAmount];
    
    readModelNodeTree(baseNode, scene->mRootNode, scene, scene->mRootNode->mTransformation);
    
    this->baseNode = baseNode;
    
    buildBuffers();
}

void Model::readModelNodeTree(MeshNode* node, aiNode* assimpNode, const aiScene* scene, aiMatrix4x4 carriedTransform) {
    unsigned int* mIndices;
    aiMesh* currentAiMesh;
    Mesh* currentSnowMesh;
    MeshNode* newNode;
    aiMatrix4x4 newTransform;
    
    node->meshCount = assimpNode->mNumMeshes;
    
    if (assimpNode->mNumMeshes > 0) {
        node->meshes = new Mesh*[node->meshCount];
        
        mIndices = assimpNode->mMeshes;
        
        for (int i = 0; i < node->meshCount; i++) {
            currentAiMesh = scene->mMeshes[mIndices[i]];
            currentSnowMesh = new Mesh;
            setMesh(currentSnowMesh, currentAiMesh, scene);
            node->meshes[i] = currentSnowMesh;
        }
    }
    newTransform = carriedTransform;
    newTransform *= assimpNode->mTransformation;
    setMeshNodeTransformation(newTransform, node);
    
    printf("%d\n", assimpNode->mNumChildren);
    
    for (int i = 0; i < assimpNode->mNumChildren; i++) {
        newNode = new MeshNode;
        node->AddChild(newNode);
        readModelNodeTree(newNode, assimpNode->mChildren[i], scene, newTransform);
    }
}

void Model::setMesh(Mesh* mesh, aiMesh* assimpMesh, const aiScene* scene) {
    aiVector3D currentAiVertex, currentAiNormal;

    for (int i = 0; i < assimpMesh->mNumVertices; i++) {
        currentAiVertex = assimpMesh->mVertices[i];
        currentAiNormal = assimpMesh->mNormals[i];
        
        vertices[i + numVertices].x = currentAiVertex.x;
        vertices[i + numVertices].y = currentAiVertex.y;
        vertices[i + numVertices].z = currentAiVertex.z;
        
        normals[i + numVertices].x = currentAiNormal.x;
        normals[i + numVertices].y = currentAiNormal.y;
        normals[i + numVertices].z = currentAiNormal.z;
        
        if (assimpMesh->HasTextureCoords(0)) {
            textureVertices[i + numVertices].x = assimpMesh->mTextureCoords[0][i].x;
            textureVertices[i + numVertices].y = assimpMesh->mTextureCoords[0][i].y;
            numTextVertices++;
        }
    }
    
    mesh->indices = new UInt32[assimpMesh->mNumFaces * 3];
    
    for (int i = 0; i < assimpMesh->mNumFaces; i++) {
        aiFace* currentFace;
        
        currentFace = &assimpMesh->mFaces[i];
        
        for (int j = 0; j < currentFace->mNumIndices; j++)
            mesh->indices[mesh->indexAmount + j] = currentFace->mIndices[j] + (UInt32)numVertices;
        mesh->indexAmount += currentFace->mNumIndices;
    }
    
    numVertices += assimpMesh->mNumVertices;
}

void Model::getVertexAmount(const aiScene* scene) {
    for (int i = 0; i < scene->mNumMeshes; i++)
        totalVertexAmount += scene->mMeshes[i]->mNumVertices;
}

void Model::setMeshNodeTransformation(aiMatrix4x4 transform, MeshNode* node) {
    node->transformation.columns[0][0] = transform.a1;
    node->transformation.columns[0][1] = transform.b1;
    node->transformation.columns[0][2] = transform.c1;
    node->transformation.columns[0][3] = transform.d1;
    
    node->transformation.columns[1][0] = transform.a2;
    node->transformation.columns[1][1] = transform.b2;
    node->transformation.columns[1][2] = transform.c2;
    node->transformation.columns[1][3] = transform.d2;
    
    node->transformation.columns[2][0] = transform.a3;
    node->transformation.columns[2][1] = transform.b3;
    node->transformation.columns[2][2] = transform.c3;
    node->transformation.columns[2][3] = transform.d3;
    
    node->transformation.columns[3][0] = transform.a4;
    node->transformation.columns[3][1] = transform.b4;
    node->transformation.columns[3][2] = transform.c4;
    node->transformation.columns[3][3] = transform.d4;
}

void Model::buildBuffers() {
    MeshNode* currentMeshNode = nullptr;
    MeshNode* stack[256];
    for (int i = 0; i < 256; i++)
        stack[i] = nullptr;
    int stackPtr = 0;
    
    const size_t sizeOfVertexBuffer = numVertices * sizeof(simd::float3);
    const size_t sizeOfNormalBuffer = sizeOfVertexBuffer;
    const size_t sizeOfTxtVtxBuffer = numTextVertices * sizeof(simd::float2);
    
    normalsBuffer = device->newBuffer(sizeOfNormalBuffer, MTL::ResourceStorageModeManaged);
    vertexBuffer = device->newBuffer(sizeOfVertexBuffer, MTL::ResourceStorageModeManaged);
    textureBuffer = device->newBuffer(sizeOfTxtVtxBuffer, MTL::ResourceStorageModeManaged);
    
    memcpy(normalsBuffer->contents(), normals, sizeOfNormalBuffer);
    memcpy(vertexBuffer->contents(), vertices, sizeOfVertexBuffer);
    memcpy(textureBuffer->contents(), textureVertices, sizeOfTxtVtxBuffer);
    
    stack[0] = baseNode;
    stackPtr++;
    
    while (stackPtr > 0) {
        for (int i = 0; i < stack[stackPtr - 1]->meshCount; i++) {
            Mesh* currentMesh = stack[stackPtr - 1]->meshes[i];
            const size_t sizeOfIndexBuffer = currentMesh->indexAmount * sizeof(UInt32);
    
            currentMesh->indexBuffer = device->newBuffer(sizeOfIndexBuffer, MTL::ResourceStorageModeManaged);
            memcpy(currentMesh->indexBuffer->contents(), currentMesh->indices, sizeOfIndexBuffer);
    
            currentMesh->indexBuffer->didModifyRange(NS::Range::Make( 0, currentMesh->indexBuffer->length() ));
        }
        
        currentMeshNode = stack[stackPtr - 1];
        stack[stackPtr - 1] = nullptr;
        stackPtr--;
        
        for (int i = 0; i < currentMeshNode->childrenCount; i++) {
            stack[stackPtr] = dynamic_cast<MeshNode*>(currentMeshNode->children[i]);
            stackPtr++;
        }
    }

    normalsBuffer->didModifyRange(NS::Range::Make(0, normalsBuffer->length()));
    vertexBuffer->didModifyRange(NS::Range::Make(0, vertexBuffer->length()));
    textureBuffer->didModifyRange(NS::Range::Make(0, textureBuffer->length()));
}

void Model::render(MTL::RenderCommandEncoder* pEnc, Snow_Uniforms* uniforms, Snow_PhongUniforms* phongUniforms) {
    MeshNode* currentMeshNode = nullptr;
    MeshNode* stack[256];
    for (int i = 0; i < 256; i++)
        stack[i] = nullptr;
    int stackPtr = 0;
    
    pEnc->setVertexBuffer(vertexBuffer, 0, 0);
    pEnc->setVertexBuffer(normalsBuffer, 0, 1);
    pEnc->setVertexBuffer(textureBuffer, 0, 2);
    
    
    pEnc->setFragmentTexture(texture->texture, 0);
    pEnc->setFragmentBytes(phongUniforms, sizeof(Snow_PhongUniforms), NS::UInteger(1));
    
    stack[0] = baseNode;
    stackPtr++;
    
    uniforms->modelMatrix = TransformMatrix();
    
    Snow_Uniforms* localUniforms = new Snow_Uniforms;
    
    while (stackPtr > 0) {
        
        simd::float4x4 translationMatrix;
        
        translationMatrix = uniforms->modelMatrix;
        translationMatrix *= stack[stackPtr - 1]->transformation;
        
        localUniforms->modelMatrix = translationMatrix;
        localUniforms->projectionMatrix = uniforms->projectionMatrix;
        localUniforms->viewMatrix = uniforms->viewMatrix;
        pEnc->setVertexBytes(localUniforms, sizeof(Snow_Uniforms), 3);
        
        for (int i = 0; i < stack[stackPtr - 1]->meshCount; i++) {
            Mesh* currentMesh = stack[stackPtr - 1]->meshes[i];
            pEnc->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, currentMesh->indexAmount, MTL::IndexTypeUInt32, currentMesh->indexBuffer, 0);
        }
        
        currentMeshNode = stack[stackPtr - 1];
        stack[stackPtr - 1] = nullptr;
        stackPtr--;
        
        for (int i = 0; i < currentMeshNode->childrenCount; i++) {
            stack[stackPtr] = static_cast<MeshNode*>(currentMeshNode->children[i]);
            stackPtr++;
        }
    }
    
    delete localUniforms;
}

void Model::Draw(MTL::RenderCommandEncoder* pEnc, Snow_Uniforms* uniforms, Snow_PhongUniforms* phongUniforms) {
    render(pEnc, uniforms, phongUniforms);
}

