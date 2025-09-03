#include "00. Global.fx"

float3 LightDir;

float4 LightSpecular;
float4 MaterialSpecular;

Texture2D DiffuseMap;

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







/*  �� Specular(�ݻ籤)
     - �ѹ������� �ݻ�Ǵ� ��
     - Phong ��� ���. (���������� Blinn-Phong ����� ���� ����Ѵ� ��)
*/

float4 PS(MeshOutput input) : SV_TARGET
{
    // float3 R = reflect(LightDir, input.normal); // -> phong �𵨷� �ݻ籤 ���ϴ� �ڵ�.. (blin phong �ƴ�)
    
    float3 R = LightDir + 2 * dot(input.normal, -LightDir) * input.normal;
    R = normalize(R); // input.normal �� normalized ���� ���� �� ������, normalize �ѵ�
    
    float3 cameraPosition = -V._41_42_43; // V �� tx, ty, tz
    float3 E = normalize(cameraPosition - input.worldPosition);
    
    float value = saturate(dot(R, E)); // �� saturate : clamp(0~1)
    float specular = pow(value, 10);
    
    float4 color = LightSpecular * MaterialSpecular * specular;
    
    return color;
}



technique11 T0
{
    PASS_VP(P0, VS, PS)
}
