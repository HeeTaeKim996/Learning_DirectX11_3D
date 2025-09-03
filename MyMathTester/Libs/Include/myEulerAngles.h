#pragma once


class myEulerAngles
{
public:
	// ������
	myEulerAngles();
	myEulerAngles(float InYaw, float InPitch, float InRoll) : Yaw(InYaw), Pitch(InPitch), Roll(InRoll) {}

	// �Լ�
	void Clamp();

	float GetAxisClampValue(float InRotatorValue);

	const static myEulerAngles Identity;


public:
	// ��� 
	float Yaw;
	float Pitch;
	float Roll;
};

