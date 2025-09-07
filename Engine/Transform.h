#pragma once
#include "Component.h"
/*----------------------------------------
			 ���� ���� �ڵ�
----------------------------------------*/

/*
class Transform : public Component
{
	using Super = Component;
public:
	Transform();
	~Transform();

	virtual void Awake() override;
	virtual void Update() override;

	void UpdateTransform();

	// Local
	Vec3 GetLocalScale() { return _localScale; }
	void SetLocalScale(const Vec3& localScale) { _localScale = localScale; UpdateTransform(); }
	Vec3 GetLocalRotation() { return _localRotation; }
	void SetLocalRotation(const Vec3& localRotation) { _localRotation = localRotation; UpdateTransform(); }
	Vec3 GetLocalPosition() { return _localPosition; }
	void SetLocalPosition(const Vec3& localPosition) { _localPosition = localPosition; UpdateTransform(); }

	// World
	Vec3 GetScale() { return _scale; }
	void SetScale(const Vec3& scale);
	Vec3 GetRotation() { return _rotation; }
	void SetRotation(const Vec3& rotation);
	Vec3 GetPosition() { return _position; }
	void SetPosition(const Vec3& position);

	Vec3 GetRight() { return _matWorld.Right(); }
	Vec3 GetUp() { return _matWorld.Up(); }
	Vec3 GetLook() { return _matWorld.Backward(); } 
	// �� SimpleMath�� �޼���ǥ���ε�, Forward�� Z -1, Backward �� Z +1 �� �ξ���
	//   � �ǵ������� ��Ȯ���𸣰�����, ��ġ ������ ��ǥ��� ������ ȿ���ε���. 
	//   ���� ���ӿ�����  �޼���ǥ���̱� ������, Forward�� �ƴ�, Backward ���



	Matrix GetWorldMatrix() { return _matWorld; }

	// ���� ����
	bool HasParent() { return _parent != nullptr; }

	shared_ptr<Transform> GetParent() { return _parent; }
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }

	const vector<shared_ptr<Transform>>& GetChildren() { return _children; }
	void AddChild(shared_ptr<Transform> child) { _children.push_back(child); }

private:
	Vec3 _localScale = { 1.f, 1.f, 1.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localPosition = { 0.f, 0.f, 0.f };

	// Cache
	Matrix _matLocal = Matrix::Identity;
	Matrix _matWorld = Matrix::Identity;

	Vec3 _scale;
	Vec3 _rotation;
	Vec3 _position;

private:
	shared_ptr<Transform> _parent;
	vector<shared_ptr<Transform>> _children;
};
*/









































































/*-------------------------------------------
		MyMath Library ��� ��ü �ڵ�
-------------------------------------------*/

class Transform : public Component
{
	using Super = Component;
public:
	// ������
	Transform();
	Transform(const TransformBase& InLocalTransformBase);


public:
	// ���� Ʈ������ ���� �Լ�
	TransformBase& GetLocalTransform() { return _localTransformBase; }
	const TransformBase& GetLocalTransform() const { return _localTransformBase; }
	void SetLocalTransform(const TransformBase& InTransformBase);


	myVec3 GetLocalPosition() const { return _localTransformBase.GetPosition(); }
	void SetLocalPosition(const myVec3& InPosition);
	void AddLocalPosition(const myVec3& InDeltaPosition);
	

	myMatrix3x3 GetLocalRotationMatrix();
	myVec3 GetLocalEulerAngles() { return _localTransformBase.GetEulerAngles().ToVec3(); }
	myVec3 GetLocalXAxis() const { return _localTransformBase.GetXAxis(); }
	myVec3 GetLocalYAxis() const { return _localTransformBase.GetYAxis(); }
	myVec3 GetLocalZAxis() const { return _localTransformBase.GetZAxis(); }

	void SetLocalRotation(const myVec3& InEulerAngles);
	void SetLocalRotation(const myQuaternion& InQuaternion);
	void SetLocalRotation(const myMatrix3x3& InMatrix);

	void AddLocalYawRotation(float InDegree);
	void AddLocalPitchRotation(float InDegree);
	void AddLocalRollRotation(float InDegree);

	void AddLocalUnitXRotation(float InDegree);
	void AddLocalUnitYRotation(float InDegree);
	void AddLocalUnitZRotation(float InDegree);

	myVec3 GetLocalScale() const { return _localTransformBase.GetScale(); }
	void SetLocalScale(const myVec3& InScale);



	myMatrix4x4 GetLocalSRT() const { return _localTransformBase.GetSRT(); }


public:
	// ���� Ʈ������ ���� �Լ�
	TransformBase& GetTransform() { return _worldTransformBase; }
	const TransformBase& GetTransform() const { return _worldTransformBase; }
	void SetTransform(const TransformBase& InTransform);

	myVec3 GetPosition() const { return _worldTransformBase.GetPosition(); }
	void SetPosition(const myVec3& InPosition);
	void AddPosition(const myVec3& InDeltaPosition);


	myMatrix3x3 GetRotationMatrix();
	myVec3 GetEulerAngles() { return _worldTransformBase.GetEulerAngles().ToVec3(); }
	myVec3 GetXAxis() const { return _worldTransformBase.GetXAxis(); }
	myVec3 GetYAxis() const { return _worldTransformBase.GetYAxis(); }
	myVec3 GetZAxis() const { return _worldTransformBase.GetZAxis(); }


	void SetRotation(const myVec3& InEulerAngles);
	void SetRotation(const myQuaternion& InQuaternion);
	void SetRotation(const myMatrix3x3& InMatrix);
	

	void AddYawRotation(float InDegree);
	void AddPitchRotation(float InDegree);
	void AddRollRotation(float InDegree);

	void AddUnitXRotation(float InDegree);
	void AddUnitYRotation(float InDegree);
	void AddUnitZRotation(float InDegree);

	myVec3 GetScale() const { return _worldTransformBase.GetScale(); }
	void SetScale(const myVec3& InScale);



	myMatrix4x4 GetSRT() const { return _worldTransformBase.GetSRT(); }


public:
	// ���� ���� ���� �Լ�
	bool SetRoot();
	bool RemoveFromParent();
	Transform& GetRoot();
	bool SetParent(Transform& InTransform);

	bool HasParent() const { return _parentPtr != nullptr; }
	vector<Transform*>& GetChildren() { return _childPtr; }
	vector<Transform*>::const_iterator ChildBegin() const { return _childPtr.begin(); }
	vector<Transform*>::const_iterator ChildEnd() const { return _childPtr.end(); }


private:
	// ���ο����� ȣ���ϴ� �Լ�
	Transform* GetParentPtr() const { return _parentPtr; }
	void UpdateLocal();
	void UpdateWorld();
	void UpdateChildrenWorld();


private:
	// ��� �Լ�
	TransformBase _localTransformBase;
	TransformBase _worldTransformBase;

	Transform* _parentPtr = nullptr;
	vector<Transform*> _childPtr;
};
