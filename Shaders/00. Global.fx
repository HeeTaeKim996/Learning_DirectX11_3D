#ifndef _GLOBAL_FX_
#define _GLOBAL_FX_

/*---------------------
       ConstBuffer
---------------------*/
cbuffer GlobalBuffer
{
    matrix V;
    matrix P;
    matrix VP;
    matrix VInv;
};

cbuffer TransformBuffer
{
    matrix W;
};



/*-------------------------
         VertexData
-------------------------*/
struct Vertex
{
    float4 position : POSITION;
};

struct VertexTextureData
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexColor
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VertexTextureNormal
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VertexTextureNormalTanget
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};


/*-------------------------
        VertexOutput
-------------------------*/
struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCORRD;
    float3 normal : NORMAL;
};

struct MeshOutput
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION1;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

/*------------------------
        SamplerState
------------------------*/
SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    /*  �� MIN_MAG_MIP_LINEAR
         - Minification     : ��ҽ� 4�� �ؼ��� ���� ����
         - Magnification    : ���������� Ȯ��� 4�� �ü��� ���� ����
         - MipMap �ܰ�       : �ؽ��ĸ� �̸� ���� �ػ󵵿����� �ؽ��ĸ� �̸� �����ϰ�, �������� �ش� �ػ󵵿� ���� �����
                               �̸� �غ�� �ؽ��ĸ� ����ϴ� ���
         - MIP_LINEAR        : ���� �������� (550, 550) �̰�, �̸� �غ�� �� (500, 500), (600, 600) �̶��,
                               �� �ػ��� �ؽ��ĸ� ���� �����Ͽ� ó��.

        - MIN_MAG_MIP_LINEAR    : ���� ������ MIN, MAG, MIPMAP �� �� ��, ��� ������ �߻��� �� �ִµ�, �̸� �����ϰ���,
                                  ���� �����Ͽ� ��� ������ ����
    */
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;
    /*  �� MIN_MAG_MIP_POINT
         - �ռ� MIN, MAG, MIP �� �������� ���� �ʰ�, ���� ����� ���� �ؼ����� ���. 
         - ��� ������ �߻��� �� ������, ���� ������ ����
    */
    AddressU = Wrap;
    AddressV = Wrap;
};
/*
    - AddressU, V �� ��κ� ���� ������ ���� (���� �����ؼ� �ܿ� �ʿ� ����)
    - Wrap      : ���ø��ϴ� ��ǥ�� ������ UV ��ǥ�� ����� ���, x' = x % maxX , y' = y % maxY, �� x', y'�� 
                  ��ȯ�Ͽ� UV ����. ( ���� ���ظ� ���Ѱ���, maxX == maxY == 1 )
    - Mirr      :  uv ��ǥ�� ����� ���, �ռ� �Ͱ� ��Ī�� �ǰ�? ó��. (������ ��Ȯ�� �𸣰�����, �������� ��)
    - Clamp     : ����Ʈ. UV ��ǥ�� ����� ���, �̻��ϰ�(?) �׸�
    - Border    : UV ��ǥ�� ����� ���, ������ ������ BorderColor ��� �׸�
*/


/*--------------------------
       RasterizerState
--------------------------*/
RasterizerState FillModeWireFrame
{
    FillMode = WireFrame;
};



/*-----------------
       Macro
-----------------*/
#define PASS_VP(name, vs, ps)                       \
pass name                                           \
{                                                   \
    SetVertexShader(CompileShader(vs_5_0, vs()));   \
    SetPixelShader(CompileShader(ps_5_0, ps()));    \
}






/*------------------
      Function
------------------*/
float3 CameraPosition()
{
    return VInv._41_42_43;
}



#endif
/*
    �� ������� �� #praga once
     - �� #ifndef #define #endif �� ������� �����̶� �Ѵ� (#pragma once �� ������ ������ ���Ǵ� ����)
     - #include �� �ش� Ŭ���� ������ �ؽ�Ʈ�� �ܼ��ϰ� '����' �ϴ� ������ �Ѵ�.
     - B, C Ŭ�������� ���� AŬ������ #include �ϰ�, Main ���� B, C Ŭ������ #include �Ѵٸ�,
       A Ŭ������ �ؽ�Ʈ�� main �� �ι� ���ٵǱ� ������, ������ �߻��Ѵ�.
     - #praga once �� ������ ��, ��ó�� ������带 �Ͽ�, �ش� ������ �����ߴ�. #pragma once �� ������带 ���� �ʾƵ�,
       �ش� ������ �������ش�
     - fx ���� ��� ���带 �ϴ� ������, fx ���� #pragma once �� ��� ���ϱ� ����
*/
