#pragma once



struct ModelKeyframeData
{
	float time;
	Vec3 scale;
	Quaternion rotation;
	Vec3 translation;
};

struct ModelKeyframe
{
	wstring boneName;
	vector<ModelKeyframeData> transforms;
};

struct ModelAnimation
{
	inline shared_ptr<ModelKeyframe> GetKeyframe(const wstring& name)
	{
		auto it = keyframes.find(name);
		if (it == keyframes.end())
		{
			return nullptr;
		}
		return it->second;
	}

	wstring name;
	float duration = 0.f;
	float frameRate = 0.f;
	uint32 frameCount = 0;
	unordered_map<wstring, shared_ptr<ModelKeyframe>> keyframes;

	// �� wstring ���� �˻��ϴ� ȿ���� �ʹ� ��������, unordered_map �� �ƴ� vector �� �ϰ�, index �� �˻��ϵ��� �� ��
	//   ��������, ������ Model ������ Bone �� Index �� �����ǰ�, AssimTool ������Ʈ���� �ִϸ��̼� ������ ������
	//   ���� ���� ������ node->mChildren(?) ������ �ִϸ��̼��� ����Ǵ�, ModelAnimation �� keyframes �� ����Ǵ�
	//   �ִϸ��̼��� ������, ���� �ε��� ������ ���� �ٵ�
	//   �� �켱 ���Ǵ�� ������, ���� �����Ѵٸ�, �ٸ� ������� �����غ���
};

