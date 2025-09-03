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
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
	Vec3 upDirection = GetTransform()->GetUp();
		
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	// ������� �̷��� ���Ѵٴµ�, ������ ��������? �� �̷��� �ұ�. 123123 �� Rot ��ġ�ϰ�, 4 �� <Vi,t> 3���ϸ� �Ǵµ�

	//S_MatView = GetTransform()->GetWorldMatrix().Invert(); 
	// �� NDC�� ��ǥ�谡 ������, ī�޶��� SRT ��Ʈ������ ����� ������ ���� �� �ֱ� �ѵ�, ���� ������ ���� �ʾ� ���δ�



	S_MatProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);  

	S_MatVInv = GetTransform()->GetWorldMatrix();
	/* 
		- Transform �ڵ尡 �ʹ� �����̶�, �̰� �����ϴ�. VInv �� ī�޶��� RT �ε�, GetWorldMattrix �� SRT
		  ���⼭ R�� �������� Decompose�� S �� �����ؾ� ��������, ���ʿ� GetRight �ڵ� ���� �׳� _11_12_13 ��
		  �����ڴ�, ������ �ڵ��.
		- ���ǿ��� Transform �� �ʹ� ����������, ���߿� ����� ����� ���ڴ� ������, ������ �׳� ����
	*/
}
