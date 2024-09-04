//
//  PerspectiveCamera.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 27/08/24.
//

#ifndef PerspectiveCamera_hpp
#define PerspectiveCamera_hpp

#include "../Cameras/Camera.hpp"

class PerspectiveCamera : public Camera{
    public:
    inline PerspectiveCamera(){};
    float fieldOfView = 60.f;
    simd_float4x4 ProjectionMatrix() override{
        simd::float4x4 projection;

        float top = near * tan((fieldOfView * SNOWSOUP_PI_F / 180.f) * 0.5);
        float bottom = -top;
        float right = top * aspectRatio;
        float left = -right;
        
        float x = 2*near/(right-left);
        float y = 2*near/(top-bottom);
        float z = -(far+near)/(far-near);
        float w = 2*far*near/(near-far);

        simd::float4 X = { x, 0, 0, 0 };
        simd::float4 Y = { 0, y, 0, 0 };
        simd::float4 Z = { 0, 0, z, -1 };
        simd::float4 W = { 0, 0, w, 0 };

        projection.columns[0] = X;
        projection.columns[1] = Y;
        projection.columns[2] = Z;
        projection.columns[3] = W;

        return projection;
    };
};

#endif /* PerspectiveCamera_hpp */
