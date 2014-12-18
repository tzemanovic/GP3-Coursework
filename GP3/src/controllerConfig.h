#pragma once

struct ControllerConfig
{
public:
	ControllerConfig( )
		: maxSpeed( 1.0f ),
		minSpeed( 0.025f ),
		accel( 0.25f ),
		decel( 0.01f ),
		maxTurnRightLeftSpeed( 2.0f ),
		turnRightLeftAccel( 1.0f ),
		turnRightLeftDecel( 0.05f ),
		maxTurnUpDownSpeed( 2.0f ),
		turnUpDownAccel( 1.0f ),
		turnUpDownDecel( 0.05f )
	{
	}
public:
	float maxSpeed;
	float minSpeed;
	float accel;
	float decel;
	float maxTurnRightLeftSpeed;
	float turnRightLeftAccel;
	float turnRightLeftDecel;
	float maxTurnUpDownSpeed;
	float turnUpDownAccel;
	float turnUpDownDecel;
};

struct ControllerState
{
	float speed;
	float turnRightLeftSpeed;
	float turnUpDownSpeed;
	bool isBirdsEye;
};