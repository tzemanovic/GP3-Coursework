#pragma once

struct ControllerConfig
{
public:
	ControllerConfig( )
		: maxSpeed( 20.0f ),
		minSpeed( 0.5f ),
		accel( 5.0f ),
		decel( 2.0f ),
		maxTurnRightLeftSpeed( 1.0f ),
		turnRightLeftAccel( 10.0f ),
		turnRightLeftDecel( 0.1f ),
		maxTurnUpDownSpeed( 1.0f ),
		turnUpDownAccel( 10.0f ),
		turnUpDownDecel( 0.1f )
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
};