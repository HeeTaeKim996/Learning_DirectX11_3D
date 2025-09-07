#pragma once
#include "myVec4.h"


class myMatrix4x4
{
public:
	// ������
	myMatrix4x4();
	myMatrix4x4(const myVec4& array0, const myVec4& array1, const myVec4& array2, const myVec4& array3);
	
	// ������
	const myVec4& operator[](BYTE index) const;
	myVec4& operator[](BYTE index);

	myMatrix4x4 operator*(float scalar) const;
	myMatrix4x4 operator*(const myMatrix4x4& InMatrix) const;

	void SetIdentity();
	myMatrix4x4 Transpose() const;


public:
	const static myMatrix4x4 Identity;

public:
	// ����Լ�
	myVec4 Arrays[4];
};
