#include "pch.h"
#include "myQuaternion.h"
#include "myMatrix3x3.h"
#include "myEulerAngles.h"


const myQuaternion myQuaternion::Identity;


myQuaternion::myQuaternion() : X(0.f), Y(0.f), Z(0.f), W(0.f)
{
}

myQuaternion::myQuaternion(const myVec3& axis, float degree)
{
	FromAxisAngle(axis, degree);
}

myQuaternion::myQuaternion(const myEulerAngles& eulerAngles)
{
	FromEulerAngles(eulerAngles);
}

myQuaternion::myQuaternion(const myVec3& lookingVector, const myVec3& upVector)
{
	FromLookingVector(lookingVector, upVector);
}

myQuaternion::myQuaternion(const myMatrix3x3& InMatrix)
{
	FromMatrix(InMatrix);
}



void myQuaternion::FromAxisAngle(const myVec3& InAxis, float inAngleDegree)
{
	float sin = 0.f, cos = 0.f;
	Math::GetSinCos(sin, cos, inAngleDegree * 0.5f);
	W = cos;
	X = sin * InAxis.X;
	Y = sin * InAxis.Y;
	Z = sin * InAxis.Z;
}

void myQuaternion::FromEulerAngles(const myEulerAngles& InEulerAngles)
{
	float cy = 0.f, sy = 0.f, cp = 0.f, sp = 0.f, cr = 0.f, sr = 0.f;
	Math::GetSinCos(sy, cy, InEulerAngles.Yaw);
	Math::GetSinCos(sp, cp, InEulerAngles.Pitch);
	Math::GetSinCos(sr, cr, InEulerAngles.Roll);

#ifdef YAW_PITCH_ROLL
	W = cy * cp * cr + sy * sp * sr;
	X = cy * cr * sp + cp * sy * sr;
	Y = cp * cr * sy - cy * sp * sr;
	Z = cy * cp * sr - cr * sy * sp;
#elif defined(ROLL_PITCH_YAW)
	W = cy * cp * cr - sy * sp * sr;
	X = cy * cr * sp - cp * sy * sr;
	Y = cp * cr * sy + cy * sp * sr;
	Z = cy * cp * sr + cr * sy * sp;

#else
	assert(0); // �÷��� �ּ� 1�� �ʿ�
#endif
}

void myQuaternion::FromMatrix(const myMatrix3x3& InMatrix)
{
	float root = 0.f;
	float trace = InMatrix[0][0] + InMatrix[1][1] + InMatrix[2][2];

	if (!Math::EqualsInTolerance(InMatrix[0].SizeSquared(), 1.f) ||
		!Math::EqualsInTolerance(InMatrix[1].SizeSquared(), 1.f) ||
		!Math::EqualsInTolerance(InMatrix[2].SizeSquared(), 1.f))
	{
		*this = myQuaternion::Identity;
	}

	if (trace > 0.f)
	{
		// W ��Ҹ� ���ϰ�, ������ ��� ���
		root = sqrtf(trace + 1.f);
		W = 0.5f * root;
		root = 0.5f / root;

		X = (InMatrix[1][2] - InMatrix[2][1]) * root;
		Y = (InMatrix[2][0] - InMatrix[0][2]) * root;
		Z = (InMatrix[0][1] - InMatrix[1][0]) * root;
	}
	else
	{
		BYTE i = 0;

		// X, Y, Z �� ���� ū ��Ҹ� �ľ�
		if (InMatrix[1][1] > InMatrix[0][0]) i = 1;
		if (InMatrix[2][2] > InMatrix[1][1]) i = 2;


		// i, j, k �� ���� ����
		static const BYTE next[3] = { 1, 2, 0 };
		BYTE j = next[i];
		BYTE k = next[j];

		// ���� ū ����� �� ���ϱ�
		root = sqrtf(InMatrix[i][i] - InMatrix[j][j] - InMatrix[k][k] + 1.f);

		float* qt[3] = { &X, &Y, &Z };
		*qt[i] = 0.5f * root;

		root = 0.5f / root;

		// ������ �� ����� �� ���ϱ�
		*qt[j] = (InMatrix[i][j] + InMatrix[j][i]) * root;
		*qt[k] = (InMatrix[i][k] + InMatrix[k][i]) * root;

		// ������ W�� ���ϱ�
		W = (InMatrix[j][k] - InMatrix[k][j]) * root;
	}
}

void myQuaternion::FromLookingVector(const myVec3& lookingVector, const myVec3& upVector)
{
	myVec3 localX, localY, localZ;

	// ���� Z�� ����
	localZ = lookingVector.GetNormalize();
	
	// �ü� ����� ���� Y���� ������ ���
	if (Math::Abs(localZ.Y) >= (1.f - SMALL_NUMBER))
	{
		localX = myVec3::UnitX;
	}
	else
	{
		localX = upVector.Cross(localZ).GetNormalize();
	}
	localY = localZ.Cross(localX).GetNormalize(); 
	// �� �̷������δ� localY ������ GetNormalize �� �ʿ� ������, ��ǻ�� ���꿡�� ������ ���� �� �ֱ⿡, GetNormalize ó��

	FromMatrix(myMatrix3x3(localX, localY, localZ));
}




myQuaternion myQuaternion::operator-() const
{
	return myQuaternion(-X, -Y, -Z, -W);
}

myQuaternion myQuaternion::operator*(const myQuaternion& InQuaternion) const
{
	myQuaternion ret;

	myVec3 v1(X, Y, Z), v2(InQuaternion.X, InQuaternion.Y, InQuaternion.Z);

	ret.W = W * InQuaternion.W - v1.Dot(v2);
	myVec3 imaginaryPart = v1 * InQuaternion.W + v2 * W + v1.Cross(v2);
	ret.X = imaginaryPart.X;
	ret.Y = imaginaryPart.Y;
	ret.Z = imaginaryPart.Z;

	return ret;
}

myQuaternion myQuaternion::operator*=(const myQuaternion& InQuaternion)
{
	myVec3 v1(X, Y, Z), v2(InQuaternion.X, InQuaternion.Y, InQuaternion.Z);

	W = W * InQuaternion.W - v1.Dot(v2);
	myVec3 imaginaryPart = v1 * InQuaternion.W + v2 * W + v1.Cross(v2);
	X = imaginaryPart.X;
	Y = imaginaryPart.Y;
	Z = imaginaryPart.Z;

	return *this;
}

myVec3 myQuaternion::operator*(const myVec3& InVector) const
{
	return RotateVector(InVector);
}

myQuaternion myQuaternion::Slerp(const myQuaternion& startQuaternion, const myQuaternion& endQuaternion, float InRatio)
{
	// @@@ ������� 
}

myVec3 myQuaternion::RotateVector(const myVec3& InVector) const
{
	return myVec3();
}

void myQuaternion::Normalize()
{
}



myEulerAngles myQuaternion::ToEulerAngles() const
{
	return myEulerAngles();
}

myMatrix3x3 myQuaternion::ToRotateVector() const
{
	return myMatrix3x3();
}



bool myQuaternion::IsUnitQuaternion() const
{
	return false;
}
