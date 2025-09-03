#pragma once


class myEulerAngles
{
public:
	// 생성자
	myEulerAngles();
	myEulerAngles(float InYaw, float InPitch, float InRoll) : Yaw(InYaw), Pitch(InPitch), Roll(InRoll) {}

	// 함수
	void Clamp();

	float GetAxisClampValue(float InRotatorValue);

	const static myEulerAngles Identity;


public:
	// 멤버 
	float Yaw;
	float Pitch;
	float Roll;
};

