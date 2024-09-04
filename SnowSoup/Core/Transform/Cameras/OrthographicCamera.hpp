//
//  OrthographicCamera.hpp
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 27/08/24.
//

#ifndef OrthographicCamera_hpp
#define OrthographicCamera_hpp

#include "../Cameras/Camera.hpp"

class OrthographicCamera : public Camera{
    public:
    inline OrthographicCamera(){};
    float halfSize = 10.f;
    simd_float4x4 ProjectionMatrix() override{
        simd::float4x4 projection;

        float top = halfSize;
        float bottom = -top;
        float right = halfSize * aspectRatio;
        float left = -right;

        float x = 2/(right-left);
        float y = 2/(top-bottom);
        float z = -2/(far-near);
        float w = -(far+near)/(far-near);

        simd::float4 X = { x, 0, 0, 0 };
        simd::float4 Y = { 0, y, 0, 0 };
        simd::float4 Z = { 0, 0, -z, 0};
        simd::float4 W = { 0, 0, -w, 1 };

        projection.columns[0] = X;
        projection.columns[1] = Y;
        projection.columns[2] = Z;
        projection.columns[3] = W;

        return projection;
    };
};

#endif /* OrthographicCamera_hpp */
