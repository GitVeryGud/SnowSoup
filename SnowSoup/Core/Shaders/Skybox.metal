//
//  Lighting.metal
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 03/09/24.
//

#include <metal_stdlib>
using namespace metal;

struct Uniforms {
    float4x4 modelMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
};

struct SkyboxUniforms {
    float3 u_LightDir;
    float3 u_LightColor;
    float3 u_SkyColor;
    float3 u_HorizonColor;
    float3 u_GroundColor;
    float u_SunSize;
};

struct v2f
{
    float4 position [[position]];
    float3 normal;
    half3 color;
};

v2f vertex vertexSkybox( uint vertexId [[vertex_id]],
                       device const float3* positions [[buffer(0)]],
                       device const float3* normals [[buffer(1)]],
                       device const float3* color [[buffer(2)]],
                        constant Uniforms &uniforms[[buffer(3)]])
{
    float4 fixedInPosition = float4( positions[ vertexId ], 1.0);
    float4 position = uniforms.projectionMatrix * uniforms.viewMatrix * uniforms.modelMatrix * fixedInPosition;
    v2f o;
    o.position = position;
    o.normal = normals[vertexId];
    o.color = half3(0.5f, 0.5f, 0.5f);
    return o;
}

half4 fragment fragmentSkybox( v2f in [[stage_in]],
                              constant SkyboxUniforms &uniforms[[buffer(0)]])
{
    float3 normal = normalize(in.normal);
    float3 lightDir = normalize(uniforms.u_LightDir);

    float3 dayTint = float3(1.0);
    float3 dawnTint = float3(2.0, 0.7, 0.5);
    float3 nightTint = float3(0.05, 0.05, 0.1);

    float3 color;
    if (normal.y > 0.0) {
        color = mix(uniforms.u_HorizonColor, uniforms.u_SkyColor, normal.y);
    }
    else {
        color = uniforms.u_GroundColor;
    }

    float sunValue = max(dot(normal, -lightDir) - (1.0 - uniforms.u_SunSize), 0.0) / uniforms.u_SunSize;
    float3 sunColor;
    if(normal.y > 0.0) {
        sunColor = uniforms.u_LightColor * float3(pow(sunValue, 3.0));
    }
    else {
        sunColor = float3(0.0);
    }

    float3 tint;
    if(lightDir.y > 0.0) {
        tint = mix(dawnTint, nightTint, lightDir.y);
    }
    else {
        tint = mix(dawnTint, dayTint, -lightDir.y);
    }

    float4 out = float4((color + sunColor) * tint, 1.0);
    return half4( out );
}

