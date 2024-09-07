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

    Snow_ForwardState state1 = buildShaders("vertexMain", "fragmentPhong");
    Snow_ForwardState state2 = buildShaders("vertexSkybox", "fragmentSkybox");
    
    _pPSO = state1.pipelineState;
    _pDSS = state1.depthState;
    _pPSO2 = state2.pipelineState;
    _pDSS2 = state2.depthState;
    
    camera = new PerspectiveCamera();
    directionalLight = new DirectionalLight();
    skybox = new SnowSpherePrimitive(_pDevice);
    
    skybox->Sphere();
    
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
    delete skybox;
}

void Renderer::setCameraAspect(float aspect){
    if (camera != nullptr){
        camera->aspectRatio = aspect;
    }
}

void Renderer::buildMatrices() {
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
    
    if (_pRenderTargetTexture != nullptr) {_pRenderTargetTexture->release();}
    _pRenderTargetTexture = _pDevice->newTexture(targetTextureDescriptor);
    
    MTL::TextureDescriptor* depthDescriptor = MTL::TextureDescriptor::alloc()->init();
    depthDescriptor->setTextureType(MTL::TextureType2DMultisample);
    depthDescriptor->setPixelFormat(MTL::PixelFormatDepth32Float);
    depthDescriptor->setWidth(width);
    depthDescriptor->setHeight(height);
    depthDescriptor->setUsage(MTL::TextureUsageRenderTarget);
    depthDescriptor->setSampleCount(2);
    
    if (_pDepthTexture != nullptr) {_pDepthTexture->release();}
    _pDepthTexture = _pDevice->newTexture(depthDescriptor);
    
    depthDescriptor->release();
    targetTextureDescriptor->release();
}

void Renderer::drawSetup( CA::MetalDrawable* drawable) {  
    buildMatrices();
    buildSkybox();
    
    directionalLight->rotation.x = 200 + angle;
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
    skybox->Draw(pEnc, uniforms, skyUniforms);
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
    
    Snow_PhongUniforms* phongUniforms = new Snow_PhongUniforms;
    
    phongUniforms->u_AmbientLightColor = vector3(0.05f, 0.05f, 0.2f);
    phongUniforms->u_ViewPosition = camera->position;
    phongUniforms->u_LightDir = directionalLight->Direction();
    phongUniforms->u_LightColor = skyUniforms->u_LightColor;
    phongUniforms->u_SpecularIntensity = 0.1f;

    uniforms->modelMatrix = transform.TransformMatrix();
    
    if (sceneTree == nullptr)
        return;
    
    nodeStack[stackPtr] = sceneTree;
    stackPtr++;
    
    while (stackPtr > 0) {
        nodeStack[stackPtr - 1]->Update();
        nodeStack[stackPtr - 1]->Draw( pEnc, uniforms, phongUniforms );
        
        currentNode = nodeStack[stackPtr - 1];
        nodeStack[stackPtr - 1] = nullptr;
        stackPtr--;
        
        for (int i = 0; i < currentNode->childrenCount; i++)
        {
            nodeStack[stackPtr] = currentNode->children[i];
            stackPtr++;
        }
    }
    
    delete phongUniforms;
}

void Renderer::endDraw(CA::MetalDrawable* drawable) {
    pEnc->endEncoding();
    
    _pCmd->presentDrawable( drawable );
    _pCmd->commit();
    _pCmd->waitUntilCompleted();
    
    _pRpd->release();
}
