#pragma once

class myMatrix3x3;
class myVec4;

class myVec3
{
public:
	/* ������ */
	myVec3() : X(0.f), Y(0.f), Z(0.f) {}
	myVec3(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ) {}


	// ������
	float& operator[](BYTE InIndex) { assert(InIndex < 3); return (&X)[InIndex]; }
	const float& operator[](BYTE InIndex) const { assert(InIndex < 3); return (&X)[InIndex]; }

	myVec3 operator+(const myVec3& InVector) const;
	myVec3 operator-(const myVec3& InVector) const;
	myVec3 operator*(float InScale) const;
	myVec3 operator/(float InScale) const;
	myVec3& operator+=(const myVec3& InVector);
	myVec3& operator-=(const myVec3& InVector);
	myVec3& operator*=(float InScale);
	myVec3& operator/=(float InScale);
	
	myVec3 operator*(const myMatrix3x3 InMatrix) const;
	myVec3& operator*=(const myMatrix3x3 InMatrix);

	myVec3 operator-() const { return myVec3(-X, -Y, -Z); }
	myVec3 operator*(const myVec3& InVector) const;
	

	// ����Լ�
	float Size() const;
	float SizeSquared() const;
	void Normalize();
	myVec3 GetNormalize() const;
	float Dot(const myVec3& InVector) const;
	myVec3 Cross(const myVec3& InVector) const;


	myVec4 ToVec4(int lastValue = 0);

public:
	// static
	const static myVec3 UnitX;
	const static myVec3 UnitY;
	const static myVec3 UnitZ;
	const static myVec3 One;
	const static myVec3 Zero;

	
public:	
	// ���
	float X, Y, Z;

	
};

