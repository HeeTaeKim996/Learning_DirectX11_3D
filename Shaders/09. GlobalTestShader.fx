#include "00. Global.fx"


VertexOutput VS(VertexTextureNormal input)
{
    VertexOutput output;
    output.position = mul(input.position, W);
    output.position = mul(output.position, VP);

    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) W); 
    // �� RS �ܰ迡�� �ڵ����� �۽���Ƽ�� ������ �ϱ� ������, PS�� input ���� ���� �� �̹� normalized�� ������.
    //   ���� ���⼭ normalized�� ���� ���� �ʴ´�.
    
    return output;
}


// Filter = Ȯ��/��� �Ͼ�� �� �߰����� ó���ϴ� ���
// Address = UV �� 1���� ���� ��, ������ �κ��� ��� ó������


Texture2D Texture0;



float4 PS(VertexOutput input) : SV_TARGET
{
    return Texture0.Sample(LinearSampler, input.uv);
}



technique11 T0
{
    PASS_VP(P0, VS, PS)
}
