#include "pch.h"
#include "Camera.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;
Matrix Camera::S_MatVInv = Matrix::Identity;

Camera::Camera() : Super(ComponentType::Camera)
{
	_width = static_cast<float>(GAME->GetGameDesc().width);
	_height = static_cast<float>(GAME->GetGameDesc().height);
}

Camera::~Camera()
{

}

void Camera::Update()
{
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	Vec3 eyePosition = GetTransform()->GetPosition();

	//Vec3 focusPosition = eyePosition + GetTransform()->GetZAxis();
	//Vec3 upDirection = GetTransform()->GetYAxis();


	myMatrix3x3 mat = GetTransform()->GetRotationMatrix();
	Vec3 focusPosition = eyePosition + mat[2];
	Vec3 upDirection = mat[1];


	{ // @@ ���� ���� �ڵ�
		//S_MatView = SimpleMatrixToMyMatrix(::XMMatrixLookAtLH(
		//	MyVec3ToSimpleVec3(eyePosition), MyVec3ToSimpleVec3(focusPosition), MyVec3ToSimpleVec3(upDirection))
		//	);
		// ������� �̷��� ���Ѵٴµ�, ������ ��������? �� �̷��� �ұ�. 123123 �� Rot ��ġ�ϰ�, 4 �� <Vi,t> 3���ϸ� �Ǵµ�


		//S_MatView = GetTransform()->GetWorldMatrix().Invert(); 
		// �� NDC�� ��ǥ�谡 ������, ī�޶��� SRT ��Ʈ������ ����� ������ ���� �� �ֱ� �ѵ�, ���� ������ ���� �ʾ� ���δ�


		// @@@@@@@@@@@@@@@@@@@@@
		//S_MatProjection = SimpleMatrixToMyMatrix(
		//	::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far)
		//	);

		// @@@@@@@@@@@@@@@@@@@@@@
		//S_MatVInv = GetTransform()->GetSRT();
		/*
			- Transform �ڵ尡 �ʹ� �����̶�, �̰� �����ϴ�. VInv �� ī�޶��� RT �ε�, GetWorldMattrix �� SRT
			  ���⼭ R�� �������� Decompose�� S �� �����ؾ� ��������, ���ʿ� GetRight �ڵ� ���� �׳� _11_12_13 ��
			  �����ڴ�, ������ �ڵ��.
			- ���ǿ��� Transform �� �ʹ� ����������, ���߿� ����� ����� ���ڴ� ������, ������ �׳� ����
		*/
	}


	SetViewMatrix();
	SetProjectionMatrix();
	SetVInvMatrix(); // Pos ���Ϸ� ���ǿ��� ���� �Ŷ�, ��� GetTransform()->GetPosition(); �� ���������, Ȥ�� �𸣴� ó��
}

void Camera::SetViewMatrix()
{
	myMatrix3x3 rotMatrix = GetTransform()->GetRotationMatrix();
	myMatrix3x3 rotTransposedMatrix = rotMatrix.Transpose();
	myVec3 cameraInvPos = -GetTransform()->GetPosition();

	S_MatView = Matrix(
		Vec4(rotTransposedMatrix[0], 0),
		Vec4(rotTransposedMatrix[1], 0),
		Vec4(rotTransposedMatrix[2], 0),
		Vec4(rotMatrix[0].Dot(cameraInvPos), rotMatrix[1].Dot(cameraInvPos), rotMatrix[2].Dot(cameraInvPos), 1)
	);
}

void Camera::SetProjectionMatrix()
{
	float d = 1.f / tanf(_fov / 2.f);
	float a = _width / _height;
	float k = _far / (_far - _near);
	float l = (_near * _far) / (_near - _far);

	S_MatProjection = myMatrix4x4(
		myVec4(d / a, 0, 0, 0),
		myVec4(0, d, 0, 0),
		myVec4(0, 0, k, 1),
		myVec4(0, 0, l, 0)
	);
}

void Camera::SetVInvMatrix()
{
	myMatrix3x3 rotMatrix = GetTransform()->GetRotationMatrix();
	
	S_MatVInv = myMatrix4x4(
		myVec4(rotMatrix[0], 0),
		myVec4(rotMatrix[1], 0),
		myVec4(rotMatrix[2], 0),
		myVec4(GetTransform()->GetPosition(), 1)
	);
}

