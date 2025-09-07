#include "pch.h"
#include "myVec3.h"
#include "myMatrix3x3.h"
#include "myVec4.h"

const myVec3 myVec3::UnitX = myVec3(1, 0, 0);
const myVec3 myVec3::UnitY = myVec3(0, 1, 0);
const myVec3 myVec3::UnitZ = myVec3(0, 0, 1);
const myVec3 myVec3::One = myVec3(1, 1, 1);
const myVec3 myVec3::Zero = myVec3(0, 0, 0);


myVec3 myVec3::operator+(const myVec3& InVector) const
{
	return myVec3(X + InVector.X, Y + InVector.Y, Z + InVector.Z);
}

myVec3 myVec3::operator-(const myVec3& InVector) const
{
	return myVec3(X - InVector.X, Y - InVector.Y, Z - InVector.Z);
}

myVec3 myVec3::operator*(float InScale) const
{
	return myVec3(X * InScale, Y * InScale, Z * InScale);
}

myVec3 myVec3::operator/(float InScale) const
{
	assert(InScale != 0.f);

	return myVec3(X / InScale, Y / InScale, Z / InScale);
}

myVec3& myVec3::operator+=(const myVec3& InVector)
{
	X += InVector.X;
	Y += InVector.Y;
	Z += InVector.Z;

	return *this;
}

myVec3& myVec3::operator-=(const myVec3& InVector)
{
	X -= InVector.X;
	Y -= InVector.Y;
	Z -= InVector.Z;

	return *this;
}

myVec3& myVec3::operator*=(float InScale)
{
	X *= InScale;
	Y *= InScale;
	Z *= InScale;

	return *this;
}

myVec3& myVec3::operator/=(float InScale)
{
	assert(InScale != 0.f);

	X /= InScale;
	Y /= InScale;
	Z /= InScale;

	return *this;
}



myVec3 myVec3::operator*(const myMatrix3x3 InMatrix) const
{
	myMatrix3x3 tMat = InMatrix.Transpose();

	return myVec3(Dot(tMat[0]), Dot(tMat[1]), Dot(tMat[2]));
}


myVec3& myVec3::operator*=(const myMatrix3x3 InMatrix)
{
	myMatrix3x3 tMat = InMatrix.Transpose();
	
	myVec3 ret(Dot(tMat[0]), Dot(tMat[1]), Dot(tMat[2]));

	*this = ret;
	return *this;
}

myVec3 myVec3::operator*(const myVec3& InVector) const
{
	return myVec3(X * InVector.X, Y * InVector.Y, Z * InVector.Z);
}




float myVec3::Size() const
{
	return sqrtf(SizeSquared());
}

float myVec3::SizeSquared() const
{
	return X * X + Y * Y + Z * Z;
}

void myVec3::Normalize()
{
	*this = GetNormalize();
}

myVec3 myVec3::GetNormalize() const
{
	float squareSum = SizeSquared();
	if (squareSum == 1.f)
	{
		return *this;
	}
	else if (squareSum == 0.f)
	{
		return myVec3(0, 0, 0);
	}

	float invLength = Math::InvSqrt(squareSum);
	return myVec3(X * invLength, Y * invLength, Z * invLength);
}

float myVec3::Dot(const myVec3& InVector) const
{
	return X * InVector.X + Y * InVector.Y + Z * InVector.Z;
}

myVec3 myVec3::Cross(const myVec3& InVector) const
{
	return myVec3(
		Y * InVector.Z - Z * InVector.Y,
		Z * InVector.X - X * InVector.Z,
		X * InVector.Y - Y * InVector.X
	);
}

myVec4 myVec3::ToVec4(int lastValue)
{
	return myVec4(X, Y, Z, lastValue);
}
