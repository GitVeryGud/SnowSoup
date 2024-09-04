//
//  Renderer.cpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 03/09/24.
//

#include "Renderer.hpp"

Renderer::Renderer( MTL::Device* pDevice )
: _pDevice( pDevice->retain() )
{
    _pCommandQueue = _pDevice->newCommandQueue();

    Snow_ForwardState state1 = buildShaders("vertexMain", "fragmentMain");
    Snow_ForwardState state2 = buildShaders("vertexSkybox", "fragmentSkybox");
    
    _pPSO = state1.pipelineState;
    _pDSS = state1.depthState;
    _pPSO2 = state2.pipelineState;
    _pDSS2 = state2.depthState;
    
    camera = new PerspectiveCamera();
    directionalLight = new DirectionalLight();
    
    buildSphere();
    
    createDepthAndTargetTextures(512*3, 512*3);
}

Renderer::~Renderer()
{
    _pCommandQueue->release();
    _pDevice->release();
    _pVertexPositionsBuffer->release();
    _pVertexNormalsBuffer->release();
    _pVertexColorsBuffer->release();
    
    _pPSO->release();
    _pDSS->release();
    _pPSO2->release();
    _pDSS2->release();
    
    delete uniforms;
    delete camera;
    delete skyUniforms;
}

void Renderer::setCameraAspect(float aspect){
    if (camera != nullptr){
        camera->aspectRatio = aspect;
    }
}

void Renderer::buildSphere() {
    const float TAU = 6.28318548;
    
    UInt16 segments = 16;
    UInt16 rings = 8;
    float radius = 1;
    
    UInt16 verticesCount = segments * rings + 1;
    UInt16 indicesCount = ((rings - 2) * (segments - 1) * 6 + 2*(segments - 1) * 3 );
    
    simd::float3 positions[verticesCount];
    
    simd::float3 normals[verticesCount];
    
    UInt32 indices[indicesCount];
    
    simd::float3 color[] =
    //color
    {
    {1.0, 0.0, 0.0}
    };
    
    for (UInt16 i = 1; i < rings; i++) {
        float valueI = i / (float) rings;
        float angleI = valueI * SNOWSOUP_PI_F;
        float y = cos(angleI);
        float mult = sin(angleI);
        
        for (UInt16 j = 0; j < segments; j++) {
            float valueJ = j / (float) (segments - 1);
            float angleJ = valueJ * TAU;
            float x = cos(angleJ) * mult;
            float z = -sin(angleJ) * mult;
            
            positions[j + (i - 1) * segments] = simd::float3 {x * radius, y * radius, z * radius};
            
            normals[j + (i - 1) * segments] = simd::float3 {x, y, z};
        }
    }
    
    UInt16 top = (UInt16) verticesCount - 2;
    positions[verticesCount - 2 ] = simd::float3 {0, radius, 0};
    normals[verticesCount - 2 ] = simd::float3 { 0, 1, 0 };
    
    UInt16 bot = (UInt16) verticesCount - 1;
    positions[verticesCount - 1 ] = simd::float3 {0, -radius, 0};
    normals[verticesCount - 1 ] = simd::float3 { 0, -1, 0 };
    
    for (UInt16 i = 0; i < rings - 2; i++) {
        for (UInt16 j = 0; j < segments - 1; j++) {
            UInt32 one = (i * segments) + j;
            UInt32 two = ((i + 1) * segments) + j;
            UInt32 three = ((i + 1) * segments) + j + 1;
            UInt32 four = (i * segments) + j + 1;
            
            int index = (i * (segments - 1) + j) * 6;
            
            indices[index] = one;
            indices[index + 1] = two;
            indices[index + 2] = three;
            
            indices[index + 3] = one;
            indices[index + 4] = three;
            indices[index + 5] = four;
        }
    }
    
    for (UInt16 i = 0; i < segments - 1; i++) {
        indices[indicesCount - 6 * (segments - 1) + i * 3] = i;
        indices[indicesCount - 6 * (segments - 1) + i * 3 + 1] = i + 1;
        indices[indicesCount - 6 * (segments - 1) + i * 3 + 2] = top;
    }
    
    for (UInt16 i = 0; i < segments - 1; i++) {
        UInt32 offset = (rings - 2) * segments;
        
        indices[indicesCount - 3 * (segments - 1) + i * 3 ] = offset + i;
        indices[indicesCount - 3 * (segments - 1) + i * 3 + 1] = bot;
        indices[indicesCount - 3 * (segments - 1) + i * 3 + 2] = offset + i + 1;
    }

    const size_t positionsDataSize = sizeof(positions);
    const size_t colorsDataSize = sizeof(color);
    const size_t normalsDataSize = sizeof(normals);
    const size_t indexDataSize = sizeof(indices);
    
    numI = indicesCount;

    MTL::Buffer* pVertexPositionsBuffer = _pDevice->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged);
    MTL::Buffer* pVertexColorsBuffer = _pDevice->newBuffer(colorsDataSize, MTL::ResourceStorageModeManaged);
    MTL::Buffer* pVertexNormalsBuffer = _pDevice->newBuffer(normalsDataSize, MTL::ResourceStorageModeManaged);
    
    MTL::Buffer* index = _pDevice->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

    _pVertexPositionsBuffer = pVertexPositionsBuffer;
    _pVertexColorsBuffer = pVertexColorsBuffer;
    _pVertexNormalsBuffer = pVertexNormalsBuffer;
    
    _pIndexBuffer = index;

    memcpy( _pVertexPositionsBuffer->contents(), positions, positionsDataSize );
    memcpy( _pVertexNormalsBuffer->contents(), normals, normalsDataSize );
    memcpy( _pVertexColorsBuffer->contents(), color, colorsDataSize );
    memcpy( _pIndexBuffer->contents(), indices, indexDataSize );

    _pVertexPositionsBuffer->didModifyRange( NS::Range::Make( 0, _pVertexPositionsBuffer->length() ) );
    _pIndexBuffer->didModifyRange(NS::Range::Make(0, _pIndexBuffer->length()));
    _pVertexNormalsBuffer->didModifyRange(NS::Range::Make(0, _pVertexNormalsBuffer->length()));
    _pVertexColorsBuffer->didModifyRange( NS::Range::Make( 0, _pVertexColorsBuffer->length() ) );
    
    // Second mesh color buffer, change later
    simd::float3 color2[] =
    //color
    {
    {0.0, 0.0, 1.0}
    };
    
    _pVertexColors2Buffer = _pDevice->newBuffer(sizeof(color2), MTL::ResourceStorageModeManaged);
    memcpy( _pVertexColors2Buffer->contents(), color2, sizeof(color2) );
}

void Renderer::buildMatrices() {
    camera->position = vector3(0.f, 0.f, -5.f);
    uniforms->viewMatrix = camera->ViewMatrix();
    uniforms->projectionMatrix = camera->ProjectionMatrix();
}

void Renderer::buildSkybox() {
    skyUniforms->u_SkyColor = vector3(0.2f, 0.4f, 1.0f);
    skyUniforms->u_HorizonColor = vector3(0.3f, 0.7f, 0.9f);
    skyUniforms->u_GroundColor = vector3(0.2f, 0.6f, 0.2f);
    skyUniforms->u_SunSize = 0.03f;
    skyUniforms->u_LightColor = vector3(1.f, 1.f, 0.6f);
    skyUniforms->u_LightDir = vector3(0.f, 0.f, 0.f);
}

Snow_ForwardState Renderer::buildShaders(std::string vertex, std::string fragment) {
    using NS::StringEncoding::UTF8StringEncoding;

    NS::Error* pError = nullptr;
    MTL::Library* pLibrary = _pDevice->newDefaultLibrary();
    if ( !pLibrary )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }
    
    MTL::Function* pVertexFn = pLibrary->newFunction(NS::String::string(vertex.data(), UTF8StringEncoding));
    MTL::Function* pFragFn = pLibrary->newFunction( NS::String::string(fragment.data(), UTF8StringEncoding) );

    MTL::RenderPipelineDescriptor* pDesc = MTL::RenderPipelineDescriptor::alloc()->init();
    
    MTL::DepthStencilDescriptor* pDepth = MTL::DepthStencilDescriptor::alloc()->init();
    pDepth->setDepthCompareFunction(MTL::CompareFunctionLessEqual);
    pDepth->setDepthWriteEnabled(true);
    
    pDesc->setVertexFunction( pVertexFn );
    pDesc->setFragmentFunction( pFragFn );
    pDesc->colorAttachments()->object(0)->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm);
    pDesc->setRasterSampleCount(2);
    pDesc->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);

    Snow_ForwardState state = Snow_ForwardState();
    state.pipelineState = _pDevice->newRenderPipelineState( pDesc, &pError );
    state.depthState = _pDevice->newDepthStencilState(pDepth);
    
    if ( !state.pipelineState || !state.depthState )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }

    pVertexFn->release();
    pFragFn->release();
    pDesc->release();
    pDepth->release();
    pLibrary->release();
    
    return state;
}

void Renderer::createDepthAndTargetTextures(int width, int height) {
    MTL::TextureDescriptor* targetTextureDescriptor = MTL::TextureDescriptor::alloc()->init();
    targetTextureDescriptor->setTextureType(MTL::TextureType2DMultisample);
    targetTextureDescriptor->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    targetTextureDescriptor->setWidth(width);
    targetTextureDescriptor->setHeight(height);
    targetTextureDescriptor->setSampleCount(2);
    targetTextureDescriptor->setUsage(MTL::TextureUsageRenderTarget);
    
    _pRenderTargetTexture = _pDevice->newTexture(targetTextureDescriptor);
    
    MTL::TextureDescriptor* depthDescriptor = MTL::TextureDescriptor::alloc()->init();
    depthDescriptor->setTextureType(MTL::TextureType2DMultisample);
    depthDescriptor->setPixelFormat(MTL::PixelFormatDepth32Float);
    depthDescriptor->setWidth(width);
    depthDescriptor->setHeight(height);
    depthDescriptor->setUsage(MTL::TextureUsageRenderTarget);
    depthDescriptor->setSampleCount(2);
    
    _pDepthTexture = _pDevice->newTexture(depthDescriptor);
    
    depthDescriptor->release();
    targetTextureDescriptor->release();
}

void Renderer::drawSetup( CA::MetalDrawable* drawable) {
    
    angle += 0.01f;
    
    buildMatrices();
    buildSkybox();
    
    directionalLight->rotation.x = 45 * angle;
    skyUniforms->u_LightDir = directionalLight->Direction();
    
    // RENDER PASS DESCRIPTOR SETUP
    // SINCE WE ARE NOT USING METALKIT ANYMORE, WE NEED TO DO THIS MANUALLY
    
    MTL::ClearColor clearColor(255,255,255,1);
    
    _pCmd = _pCommandQueue->commandBuffer();
    _pRpd = MTL::RenderPassDescriptor::alloc()->init();
    
    MTL::RenderPassColorAttachmentDescriptor* pCad = _pRpd->colorAttachments()->object(0);
    MTL::RenderPassDepthAttachmentDescriptor* pDad = _pRpd->depthAttachment();
    
    pCad->setResolveTexture(_pRenderTargetTexture);
    pCad->setTexture(drawable->texture());
    pCad->setLoadAction(MTL::LoadActionClear);
    pCad->setClearColor(clearColor);
    pCad->setStoreAction(MTL::StoreActionMultisampleResolve);
    
    pDad->setTexture(_pDepthTexture);
    pDad->setLoadAction(MTL::LoadActionClear);
    pDad->setStoreAction(MTL::StoreActionDontCare);
    pDad->setClearDepth(1.0);
    
    _pRpd->colorAttachments()->object(0)->setTexture(_pRenderTargetTexture);
    _pRpd->colorAttachments()->object(0)->setResolveTexture(drawable->texture());
    _pRpd->depthAttachment()->setTexture(_pDepthTexture);
}

void Renderer::initDraw(CA::MetalDrawable* drawable) {
    pEnc = _pCmd->renderCommandEncoder(_pRpd);
    pEnc->setRenderPipelineState(_pPSO2);
    pEnc->setVertexBuffer(_pVertexPositionsBuffer, 0, 0);
    pEnc->setVertexBuffer(_pVertexNormalsBuffer, 0, 1);
    pEnc->setVertexBuffer(_pVertexColorsBuffer, 0, 2);
    pEnc->setVertexBytes(uniforms, sizeof(Snow_Uniforms), NS::UInteger(3));
    pEnc->setFragmentBytes(skyUniforms, sizeof(Snow_SkyboxUniforms), NS::UInteger(0));
    
    pEnc->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, numI, MTL::IndexTypeUInt32, _pIndexBuffer, 0);
    pEnc->setFrontFacingWinding(MTL::WindingCounterClockwise);
}

void Renderer::draw( CA::MetalDrawable* drawable, Node* sceneTree ) {
    Node* nodeStack[256], *currentNode;
    int stackPtr = 0;
    
    Transform transform = Transform();

    transform.position = camera->position;
    
    uniforms->modelMatrix = transform.TransformMatrix();
    
    initDraw(drawable);
    
    // DRAW MODEL
    pEnc->setRenderPipelineState(_pPSO);
    pEnc->setCullMode(MTL::CullModeBack);
    pEnc->setFrontFacingWinding(MTL::WindingCounterClockwise);
    pEnc->setDepthStencilState(_pDSS);
    
    if (sceneTree == nullptr)
        return;
    
    nodeStack[stackPtr] = sceneTree;
    stackPtr++;
    
    while (stackPtr > 0) {
        nodeStack[stackPtr - 1]->Update();
        nodeStack[stackPtr - 1]->Draw( pEnc, uniforms );
        
        currentNode = nodeStack[stackPtr - 1];
        nodeStack[stackPtr - 1] = nullptr;
        stackPtr--;
        
        for (int i = 0; i < currentNode->childrenCount; i++)
        {
            nodeStack[stackPtr] = currentNode->children[i];
            stackPtr++;
        }
    }
}

void Renderer::endDraw(CA::MetalDrawable* drawable) {
    pEnc->endEncoding();
    
    _pCmd->presentDrawable( drawable );
    _pCmd->commit();
    _pCmd->waitUntilCompleted();
    
    _pRpd->release();
}
