#include "pch.h"
#include "myMatrix4x4.h"

const myMatrix4x4 myMatrix4x4::Identity;

myMatrix4x4::myMatrix4x4()
	: Arrays{ myVec4(1, 0, 0, 0), myVec4(0, 1, 0, 0), myVec4(0, 0, 1, 0), myVec4(0, 0, 0, 1)}
{
}

myMatrix4x4::myMatrix4x4(const myVec4& array0, const myVec4& array1, const myVec4& array2, const myVec4& array3)
	: Arrays { array0, array1, array2, array3}
{
}

const myVec4& myMatrix4x4::operator[](BYTE index) const
{
	assert(index < 4);
	return Arrays[index];
}

myVec4& myMatrix4x4::operator[](BYTE index)
{
	assert(index < 4);
	return Arrays[index];
}

myMatrix4x4 myMatrix4x4::operator*(float scalar) const
{
	return myMatrix4x4(
		Arrays[0] * scalar,
		Arrays[1] * scalar,
		Arrays[2] * scalar,
		Arrays[3] * scalar
	);
}

myMatrix4x4 myMatrix4x4::operator*(const myMatrix4x4& InMatrix) const
{
	myMatrix4x4 tMat = InMatrix.Transpose();
	return myMatrix4x4(
		myVec4(Arrays[0].Dot(tMat[0]), Arrays[0].Dot(tMat[1]), Arrays[0].Dot(tMat[2]), Arrays[0].Dot(tMat[3])),
		myVec4(Arrays[1].Dot(tMat[0]), Arrays[1].Dot(tMat[1]), Arrays[1].Dot(tMat[2]), Arrays[1].Dot(tMat[3])),
		myVec4(Arrays[2].Dot(tMat[0]), Arrays[2].Dot(tMat[1]), Arrays[2].Dot(tMat[2]), Arrays[2].Dot(tMat[3])),
		myVec4(Arrays[3].Dot(tMat[0]), Arrays[3].Dot(tMat[1]), Arrays[3].Dot(tMat[2]), Arrays[3].Dot(tMat[3]))
	);
}

void myMatrix4x4::SetIdentity()
{
	*this = myMatrix4x4::Identity;
}

myMatrix4x4 myMatrix4x4::Transpose() const
{
	return myMatrix4x4(
		myVec4(Arrays[0].X, Arrays[1].X, Arrays[2].X, Arrays[3].X),
		myVec4(Arrays[0].Y, Arrays[1].Y, Arrays[2].Y, Arrays[3].Y),
		myVec4(Arrays[0].Z, Arrays[1].Z, Arrays[2].Z, Arrays[3].Z),
		myVec4(Arrays[0].W, Arrays[1].W, Arrays[2].W, Arrays[3].W)
	);
}
