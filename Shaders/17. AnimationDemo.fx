#include "00. Global.fx"
#include "00. Light.fx"


#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500



struct KeyframeDesc
{
    int animIndex;
    uint currentFrame;
    uint nextFrame;
    float ratio;
    float sumTime;
    float speed;
    float2 padding;
};

cbuffer KeyframeBuffer
{
    KeyframeDesc keyframes;
};


cbuffer BoneBuffer
{
    matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};


uint BoneIndex;
Texture2DArray TransformMap;

Matrix GetAnimationMatrix(VertexTextureNormalTangetBlend input)
{
    float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, 
        input.blendIndices.w };
    float weights[4] = {input.blendWeights.x, input.blendWeights.y, input.blendWeights.z,
        input.blendWeights.w};
    
    int animIndex = keyframes.animIndex;
    int currentFrame = keyframes.currentFrame;

    float4 c0, c1, c2, c3;
    Matrix transform = 0;
    
    for (int i = 0; i < 4; i++)
    {
        c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currentFrame, animIndex, 0));
        c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currentFrame, animIndex, 0));
        c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currentFrame, animIndex, 0));
        c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currentFrame, animIndex, 0));

        Matrix curr = matrix(c0, c1, c2, c3);
        transform += mul(weights[i], curr); 
        // ����� �й� ��Ģ�� �����ϴ�, += �ε� ���� [ (a+b+c+d)v = av +bv + cv + dv ]
    }

    return transform;
}


MeshOutput VS(VertexTextureNormalTangetBlend input)
{
    MeshOutput output;
    Matrix animationMatrix = GetAnimationMatrix(input);
    
    output.position = mul(input.position, animationMatrix);
    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz; // ���� ����. W �������� pos

    output.position = mul(output.position, VP);
    //output.position = mul(output.position, V);
    //output.position = mul(output.position, P);

    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) W); 
    // �� RS �ܰ迡�� �ڵ����� �۽���Ƽ�� ������ �ϱ� ������, PS�� input ���� ���� �� �̹� normalized�� ������.
    //   ���� ���⼭ normalized�� ���� ���� �ʴ´�.
    output.tangent = mul(input.tangent, (float3x3) W);
    
    return output;
}







float4 PS(MeshOutput input) : SV_TARGET
{
    //ComputeNormalMapping(input.normal, input.tangent, input.uv);
    
    //float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);

    float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
    
    return color;
}


float4 PS_RED(MeshOutput input) : SV_TARGET
{
    return float4(1, 1, 1, 1);

}


technique11 T0
{
    PASS_VP(P0, VS, PS)
    PASS_RS_VP(P1, FillModeWireFrame, VS, PS_RED)
}
