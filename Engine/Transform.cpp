#include "pch.h"
#include "Transform.h"


/*----------------------------------------
			 ���� ���� �ڵ�
----------------------------------------*/
/*
Transform::Transform() : Super(ComponentType::Transform)
{

}

Transform::~Transform()
{

}

void Transform::Awake()
{
}

void Transform::Update()
{
}

Vec3 ToEulerAngles(Quaternion q)
{
	Vec3 angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.y = 2 * std::atan2(sinp, cosp) - 3.14159f / 2;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

void Transform::UpdateTransform()
{
	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	_matLocal = matScale * matRotation * matTranslation;

	if (HasParent())
	{
		_matWorld = _matLocal * _parent->GetWorldMatrix();
	}
	else
	{
		_matWorld = _matLocal;
	}

	Quaternion quat;
	_matWorld.Decompose(_scale, quat, _position);
	_rotation = ToEulerAngles(quat);

	

	// Children
	for (const shared_ptr<Transform>& child : _children)
		child->UpdateTransform();
}

void Transform::SetScale(const Vec3& worldScale)
{
	if (HasParent())
	{
		Vec3 parentScale = _parent->GetScale();
		Vec3 scale = worldScale;
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;
		SetLocalScale(scale);
	}
	else
	{
		SetLocalScale(worldScale);
	}
}

void Transform::SetRotation(const Vec3& worldRotation)
{
	if (HasParent())
	{
		Matrix inverseMatrix = _parent->GetWorldMatrix().Invert();

		Vec3 rotation;
		rotation.TransformNormal(worldRotation, inverseMatrix);

		SetLocalRotation(rotation);
	}
	else
	{
		SetLocalRotation(worldRotation);
	}
}

void Transform::SetPosition(const Vec3& worldPosition)
{
	if (HasParent())
	{
		Matrix worldToParentLocalMatrix = _parent->GetWorldMatrix().Invert();

		Vec3 position;
		position.Transform(worldPosition, worldToParentLocalMatrix);

		SetLocalPosition(position);
	}
	else
	{
		SetLocalPosition(worldPosition);
	}
}
*/










































































/*-------------------------------------------
		MyMath Library ��� ��ü �ڵ�
-------------------------------------------*/



/*--------------
	  ������
---------------*/
Transform::Transform() : Super(ComponentType::Transform)
{
}

Transform::Transform(const TransformBase& InLocalTransformBase) 
	: Super(ComponentType::Transform), _localTransformBase(InLocalTransformBase)
{
	UpdateWorld();
}





/*----------------------------
	 ���� Ʈ������ ���� �Լ�
-----------------------------*/
void Transform::SetLocalTransform(const TransformBase& InTransformBase)
{
	_localTransformBase = InTransformBase;
	UpdateWorld();
}




void Transform::SetLocalPosition(const myVec3& InPosition)
{
	_localTransformBase.SetPosition(InPosition);
	UpdateWorld();
}

void Transform::AddLocalPosition(const myVec3& InDeltaPosition)
{
	_localTransformBase.AddPosition(InDeltaPosition);
	UpdateWorld();
}





myMatrix3x3 Transform::GetLocalRotationMatrix()
{
	return _localTransformBase.GetRotation().ToRotateMatrix();
}

void Transform::SetLocalRotation(const myVec3& InEulerAngles)
{
	_localTransformBase.SetRotation(myEulerAngles(InEulerAngles.X, InEulerAngles.Y, InEulerAngles.Z));
	UpdateWorld();
}

void Transform::SetLocalRotation(const myQuaternion& InQuaternion)
{
	_localTransformBase.SetRotation(InQuaternion);
	UpdateWorld();
}

void Transform::SetLocalRotation(const myMatrix3x3& InMatrix)
{
	_localTransformBase.SetRotation(InMatrix);
	UpdateWorld();
}

void Transform::AddLocalYawRotation(float InDegree)
{
	_localTransformBase.AddYawRotation(InDegree);
	UpdateWorld();
}

void Transform::AddLocalPitchRotation(float InDegree)
{
	_localTransformBase.AddPitchRotation(InDegree);
	UpdateWorld();
}

void Transform::AddLocalRollRotation(float InDegree)
{
	_localTransformBase.AddRollRotation(InDegree);
	UpdateWorld();
}


void Transform::AddLocalUnitXRotation(float InDegree)
{
	_localTransformBase.AddUnitXRotation(InDegree);
	UpdateWorld();
}

void Transform::AddLocalUnitYRotation(float InDegree)
{
	_localTransformBase.AddUnitYRotation(InDegree);
	UpdateWorld();
}

void Transform::AddLocalUnitZRotation(float InDegree)
{
	_localTransformBase.AddUnitZRotation(InDegree);
	UpdateWorld();
}









void Transform::SetLocalScale(const myVec3& InScale)
{
	_localTransformBase.SetScale(InScale);
	UpdateWorld();
}







void Transform::SetTransform(const TransformBase& InTransform)
{
	_worldTransformBase = InTransform;
	UpdateLocal();
	UpdateChildrenWorld();
}

void Transform::SetPosition(const myVec3& InPosition)
{
	_worldTransformBase.SetPosition(InPosition);
	UpdateLocal();
	UpdateChildrenWorld();
}

void Transform::AddPosition(const myVec3& InDeltaPosition)
{
	_worldTransformBase.AddPosition(InDeltaPosition);
	UpdateLocal();
	UpdateChildrenWorld();
}

myMatrix3x3 Transform::GetRotationMatrix()
{
	return _worldTransformBase.GetRotation().ToRotateMatrix();
}

void Transform::SetRotation(const myVec3& InEulerAngles)
{
	_worldTransformBase.SetRotation(myEulerAngles(InEulerAngles.X, InEulerAngles.Y, InEulerAngles.Z));
	UpdateLocal();
	UpdateChildrenWorld();
}

void Transform::SetRotation(const myQuaternion& InQuaternion)
{
	_worldTransformBase.SetRotation(InQuaternion);
	UpdateLocal();
	UpdateChildrenWorld();
}

void Transform::SetRotation(const myMatrix3x3& InMatrix)
{
	_worldTransformBase.SetRotation(InMatrix);
	UpdateLocal();
	UpdateChildrenWorld();
}

void Transform::AddYawRotation(float InDegree)
{
	_worldTransformBase.AddYawRotation(InDegree);
	UpdateLocal();
	UpdateChildrenWorld();
}

void Transform::AddPitchRotation(float InDegree)
{
	_worldTransformBase.AddPitchRotation(InDegree);
	UpdateLocal();
	UpdateChildrenWorld();
}

void Transform::AddRollRotation(float InDegree)
{
	_worldTransformBase.AddRollRotation(InDegree);
	UpdateLocal();
	UpdateChildrenWorld();
}

void Transform::AddUnitXRotation(float InDegree)
{
	_worldTransformBase.AddUnitXRotation(InDegree);
	UpdateLocal();
	UpdateChildrenWorld();
}

void Transform::AddUnitYRotation(float InDegree)
{
	_worldTransformBase.AddUnitYRotation(InDegree);
	UpdateLocal();
	UpdateChildrenWorld();
}

void Transform::AddUnitZRotation(float InDegree)
{
	_worldTransformBase.AddUnitZRotation(InDegree);
	UpdateLocal();
	UpdateChildrenWorld();
}

void Transform::SetScale(const myVec3& InScale)
{
	_worldTransformBase.SetScale(InScale);
	UpdateLocal();
	UpdateChildrenWorld();
}

bool Transform::SetRoot()
{
	if (!RemoveFromParent())
	{
		return false;
	}

	// ���� ������ ���� ������ �����Ѵ�
	UpdateLocal();

	return true;
}

bool Transform::RemoveFromParent()
{
	if (!HasParent())
	{
		return true;
	}

	Transform& parent = *GetParentPtr();
	auto it = std::find(parent.ChildBegin(), parent.ChildEnd(), this);
	if (it == parent.ChildEnd()) // @@ ���� �ڵ忡���� != �̾��µ�, �������� �� �ϴ�
	{
		// ���� �߻�
		return false;
	}

	// �θ� Ʈ���������� �ڽ� ������ ����
	parent.GetChildren().erase(it);

	// �ڽſ��Լ� �θ� ������ ����
	_parentPtr = nullptr;
	
	return true;
}

Transform& Transform::GetRoot()
{
	Transform* parent = nullptr;
	Transform* current = this;

	while ((parent = current->GetParentPtr()) != nullptr)
	{
		current = parent;
	}

	return *current;
}

bool Transform::SetParent(Transform& InTransform)
{
	// ���� ��带 �θ�κ��� �и�
	if (!SetRoot())
	{
		return false;
	}

	// ���ο� �θ��� �ڽ����� ���. �̹� �ִ� ��쿡�� ������ �ִ� ��Ȳ.	
	auto it = std::find(InTransform.ChildBegin(), InTransform.ChildEnd(), this);
	if (it != InTransform.ChildEnd())
	{
		return false;
	}

	// ���ο� Ʈ������ ���� �θ� �缳��
	InTransform.GetChildren().emplace_back(this); 
	// �� vector<Transform*> �̶��, push_back(this); �� ����� �����ϴ� �Ѵ�. unique_ptr�� �ٷ� �� �����ϴٴµ�,
	//   �׺κб��� ���������� ����. push_back ���� ����� ���δ�. �켱�� �״�� ���. @@
	_parentPtr = &InTransform;
	
	// ���ο� �θ� ���� �ڽ��� ���� ������ ������Ʈ
	UpdateLocal();

	return true;

	/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		- ���ذ� �ȵǴ� �κ��� ���� �ڵ�������, �켱 ������� ����غ���, 
		  ���� ��ü������ �����ϰ� ��������
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
}

void Transform::UpdateLocal()
{
	if (HasParent())
	{
		const Transform& parent = *GetParentPtr();
		_localTransformBase = _worldTransformBase.WorldToLocal(parent.GetTransform());
	}
	else
	{
		_localTransformBase = _worldTransformBase;
	}
}

void Transform::UpdateWorld()
{
	// �ڽ��� ���� Ʈ�������� �켱 ������Ʈ
	if (HasParent())
	{
		const Transform& parent = *GetParentPtr();
		_worldTransformBase = _localTransformBase.LocalToWorld(parent.GetTransform());
	}
	else
	{
		_worldTransformBase = _localTransformBase;
	}

	// ���� ���� ����� �ڽ��� ���� ������ ������Ʈ
	UpdateChildrenWorld();
}

void Transform::UpdateChildrenWorld()
{
	for (auto it = ChildBegin(); it != ChildEnd(); it++)
	{
		(*it)->UpdateWorld();
	}
}

