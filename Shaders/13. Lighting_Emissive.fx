#include "00. Global.fx"

float4 MaterialEmissive;

MeshOutput VS(VertexTextureNormal input)
{
    MeshOutput output;
    output.position = mul(input.position, W);
    output.worldPosition = output.position; // ���� ����. W �������� pos
    output.position = mul(output.position, VP);

    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) W); 
    // �� RS �ܰ迡�� �ڵ����� �۽���Ƽ�� ������ �ϱ� ������, PS�� input ���� ���� �� �̹� normalized�� ������.
    //   ���� ���⼭ normalized�� ���� ���� �ʴ´�.
    
    return output;
}







/*  �� Emissive (�ܰ���)
     - ������ ����������, ������Ʈ �� �Ҹ�
*/

float4 PS(MeshOutput input) : SV_TARGET
{
    float3 cameraPosition = -V._41_42_43;
    float3 E = normalize(cameraPosition - input.worldPosition);
    
    float3 temp = mul(input.normal, (float3x3)V);
    
    float value = saturate(dot(E, temp));
    float emissive = 1.0f - value;
    
    emissive = smoothstep(0.0f, 1.0f, emissive);
    // �� SmoothStep : ���� ������ �� �ʿ� ����, bezier�� ������ ������, �ε巴�� �����ϴ� �
    emissive = pow(emissive, 2);
    
    float4 color = MaterialEmissive * emissive;
    
    return color;
}



technique11 T0
{
    PASS_VP(P0, VS, PS)
}
