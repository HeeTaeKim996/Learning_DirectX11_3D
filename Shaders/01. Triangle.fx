/*
	�� Shaders fx ���� ����� ���� ����
	 - fx ���ϵ��� Client Project ���� ��ġ���� �ʰ�, $(SolutionDir)Shaders\ �� ��ġ�ϱ� ������, �߰��� ���� �ʿ�
	 - 1) Client��, Engine�̵�, hlsl ������ ���� ��, ���� -> ����
	 - 2) ������� hlsl ������, Ctrl + X ��, $(SolutionDir)Shaders\ �� �̵�
	 - 3) �̵��� hlsl ������ �巡�� �Ͽ�, Client �� Shaders ���Ϸ� �̵�
     - 4) Ȯ���ڸ� hlsl �� �ƴ�, fx�� ����

	�� fx ���� �ʱ�ȭ �۾�
	 - �ش� fx ������, ������ 3D hlsl ���Ϸ� ������
	 - �Ӽ� ->		������ �̸� : "" (����),		���̴� ���� : ȿ��(/fx),		���̴� �� : Shader Model 5.0(/5_0)
	  �� ȿ��(/fx) �� Engine �� Shader_externLibrary �� Pass, Shader, Technique.. ���� �ڵ�� ����Ǿ� ����
	 
*/


struct VertexInput
{
	float4 position : POSITION;
};

struct VertexOutput
{
	float4 position : SV_POSITION;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.position = input.position;

	return output;
}

float4 PS(VertexOutput input) : SV_TARGET
{
	return float4(1, 0, 0, 1);
}

float4 PS2(VertexOutput input) : SV_TARGET
{
	return float4(0, 1, 0, 1);
}

float4 PS3(VertexOutput input) : SV_TARGET
{
	return float4(0, 0, 1, 1);
}

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
};

technique11 T1
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS3()));
	}
};