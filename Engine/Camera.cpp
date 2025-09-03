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
	// 역행렬을 이렇게 구한다는데, 성능이 좋으려나? 왜 이렇게 할까. 123123 에 Rot 전치하고, 4 에 <Vi,t> 3번하면 되는데

	//S_MatView = GetTransform()->GetWorldMatrix().Invert(); 
	// ※ NDC랑 좌표계가 같으니, 카메라의 SRT 메트릭스를 역행렬 때려도 구할 수 있긴 한데, 연산 성능이 좋지 않아 보인다



	S_MatProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);  

	S_MatVInv = GetTransform()->GetWorldMatrix();
	/* 
		- Transform 코드가 너무 엉망이라, 이게 가능하다. VInv 는 카메라의 RT 인데, GetWorldMattrix 는 SRT
		  여기서 R을 뽑으려면 Decompose로 S 를 제외해야 할테지만, 애초에 GetRight 코드 보면 그냥 _11_12_13 을
		  때려박는, 엉망인 코드다.
		- 강의에서 Transform 을 너무 대충했으니, 나중에 제대로 만들면 좋겠다 싶지만, 지금은 그냥 하자
	*/
}
