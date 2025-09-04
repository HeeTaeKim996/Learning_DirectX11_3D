#pragma once
#include "TesterPch.h"
#include "myVec3.h"
#include "assert.h"
#include "myMatrix3x3.h"
#include "myVec4.h"
#include "myMatrix4x4.h"
#include "myEulerAngles.h"
#include "myQuaternion.h"

using namespace std;


static void PrintMatrix3x3(myMatrix3x3 matrix)
{
	cout << matrix[0][0] << "  /   " << matrix[0][1] << "  /   " << matrix[0][2] << endl;
	cout << matrix[1][0] << "  /   " << matrix[1][1] << "  /   " << matrix[1][2] << endl;
	cout << matrix[2][0] << "  /   " << matrix[2][1] << "  /   " << matrix[2][2] << endl;
	printf("\n");
}

static void PritQuaternion(myQuaternion quat)
{
	cout << quat.W << "  /   " << quat.X << "  /   " << quat.Y << "  /   " << quat.Z << endl;
	printf("\n");
}

static void PrintEulerAngles(myEulerAngles euler)
{
	cout << "Yaw : " << euler.Yaw << "/  Pitch : " << euler.Pitch << "/  Roll : " << euler.Roll << endl;
	printf("\n");
}