#include "DirectXMath.h"
#include "SimpleMath.h"
#include "MyMath/MyMath.h"


static myVec3 SimpleVec3ToMyVec3(DirectX::SimpleMath::Vector3 simpleVec3)
{
	return myVec3(simpleVec3.x, simpleVec3.y, simpleVec3.z);
}

static DirectX::SimpleMath::Vector3 MyVec3ToSimpleVec3(myVec3 myVec3)
{
	return DirectX::SimpleMath::Vector3(myVec3.X, myVec3.Y, myVec3.Z);
}

static myVec4 SimpleVec4ToMyVec4(DirectX::SimpleMath::Vector4 simpleVec4)
{
	return myVec4(simpleVec4.x, simpleVec4.y, simpleVec4.z, simpleVec4.w);
}

static DirectX::SimpleMath::Vector4 MyVec4ToSimpleVec4(myVec4 myVec4)
{
	return DirectX::SimpleMath::Vector4(myVec4.X, myVec4.Y, myVec4.Z, myVec4.W);
}


static myMatrix4x4 SimpleMatrixToMyMatrix(DirectX::SimpleMath::Matrix simpleMatrix)
{
	return myMatrix4x4(
		myVec4(simpleMatrix._11, simpleMatrix._12, simpleMatrix._13, simpleMatrix._14),
		myVec4(simpleMatrix._21, simpleMatrix._22, simpleMatrix._23, simpleMatrix._24),
		myVec4(simpleMatrix._31, simpleMatrix._32, simpleMatrix._33, simpleMatrix._34),
		myVec4(simpleMatrix._41, simpleMatrix._42, simpleMatrix._43, simpleMatrix._44)
	);
}

static DirectX::SimpleMath::Matrix MyMatrixToSimpleMatrix(myMatrix4x4 myMatrix)
{
	return DirectX::SimpleMath::Matrix(
		myMatrix[0][0], myMatrix[0][1], myMatrix[0][2], myMatrix[0][3],
		myMatrix[1][0], myMatrix[1][1], myMatrix[1][2], myMatrix[1][3],
		myMatrix[2][0], myMatrix[2][1], myMatrix[2][2], myMatrix[2][3],
		myMatrix[3][0], myMatrix[3][1], myMatrix[3][2], myMatrix[3][3]
	);
}