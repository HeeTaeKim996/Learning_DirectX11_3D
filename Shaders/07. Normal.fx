matrix World;
matrix View;
matrix Projection;
Texture2D Texture0;
float3 LightDir;

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.position = mul(input.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);

    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) World); 
    // �� RS �ܰ迡�� �ڵ����� �۽���Ƽ�� ������ �ϱ� ������, PS�� input ���� ���� �� �̹� normalized�� ������.
    //   ���� ���⼭ normalized�� ���� ���� �ʴ´�.
    
    return output;
}


// Filter = Ȯ��/��� �Ͼ�� �� �߰����� ó���ϴ� ���
// Address = UV �� 1���� ���� ��, ������ �κ��� ��� ó������


SamplerState Sampler0;
/*
    - AddressU, V �� ��κ� ���� ������ ���� (���� �����ؼ� �ܿ� �ʿ� ����)

    - Wrap      : ���ø��ϴ� ��ǥ�� ������ UV ��ǥ�� ����� ���, x' = x % maxX , y' = y % maxY, �� x', y'�� 
                  ��ȯ�Ͽ� UV ����. ( ���� ���ظ� ���Ѱ���, maxX == maxY == 1 )
    - Mirr      :  uv ��ǥ�� ����� ���, �ռ� �Ͱ� ��Ī�� �ǰ�? ó��. (������ ��Ȯ�� �𸣰�����, �������� ��)
    - Clamp     : ����Ʈ. UV ��ǥ�� ����� ���, �̻��ϰ�(?) �׸�
    - Border    : UV ��ǥ�� ����� ���, ������ ������ BorderColor ��� �׸�
*/

float4 PS(VertexOutput input) : SV_TARGET
{   
    /*  
        �� VS �� return �� ������, PS�� input �� ������ �ٸ���
          Rasterizer �ܰ迡��, barycentric (�۽���Ƽ�� ����) ������ ���� ���� ���̴�. 
          [ �̵���� ���Ӽ����� �ﰢ������ ���ɰ����� ������ ���� ��� ���� + ���� ���� ���� ( y = 1 / x �װ�..) 
            �� barycentric(�۽���Ƽ��) ������ �⺻ �����̶� ��.. ]
    */
    float3 normal = normalize(input.normal); // RS ���� ���� �� normalized�� ������ ������, ���⼭ normalized ó��
    float3 light = -LightDir;
    
    return float4(1, 1, 1, 1) * dot(light, normal);
}

float4 PS2(VertexOutput input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float3 light = -LightDir;

    return Texture0.Sample(Sampler0, input.uv) * dot(light, normal);
}

float4 PS3(VertexOutput input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float3 light = -LightDir;

    return Texture0.Sample(Sampler0, input.uv) * dot(light, normal) * 0.5f; // 0.5f �� �߰���, ��ü ��⸦ ����
}

RasterizerState FillModeWireFrame
{
    FillMode = WireFrame;
};

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
    
    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS2()));
    }

    pass P2
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS3()));
    }
}
