//
//  PhongShader.metal
//  SnowSoup
//
//  Created by Guilherme de Souza Barci on 03/09/24.
//

#include <metal_stdlib>
using namespace metal;

struct PhongUniforms {
    float3 u_LightDir;
    float3 u_AmbientLightColor;
    float3 u_LightColor;
    float3 u_ViewPosition;
    float u_SpecularIntensity;
};

struct v2f
{
    float4 position [[position]];
    float3 normal;
    float2 texturePosition;
};

half4 fragment fragmentPhong(v2f in [[stage_in]],  texture2d<float> texture [[texture(0)]],
                    constant PhongUniforms &uniforms[[buffer(1)]]){
    float3 f_Position = in.position.xyz;
    float3 f_Normal = in.normal;
    
    float3 lightDir = normalize(uniforms.u_LightDir);
    float3 viewDir = normalize(f_Position - uniforms.u_ViewPosition);
    float3 reflectDir = reflect(lightDir, normalize(f_Normal));
        
    float specular = pow(max(dot(-viewDir, reflectDir), 0.0), 32);

    constexpr sampler textureSampler (mag_filter::linear, min_filter::linear);
    half4 out_Color = half4(texture.sample(textureSampler, in.texturePosition));
    
    float intensity = max(dot(-lightDir, normalize(f_Normal)), 0.0);
    
    out_Color.rgb *= half3(
    uniforms.u_AmbientLightColor + //cor ambiente
    uniforms.u_LightColor * intensity + //cor da luz direcional
    uniforms.u_LightColor * specular * uniforms.u_SpecularIntensity) //reflexo specular
    ;
    
    return out_Color;
}
