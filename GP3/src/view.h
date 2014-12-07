#pragma once

class GameObject;
class InputMessage;
class Camera;

class View
{
public:
	View( );
	virtual ~View( );
	View( const View& ) = delete;
	View& operator=( const View& ) = delete;
public:
	virtual void vRender( const float deltaMs ) = 0;
	virtual void vUpdate( const float deltaMs ) = 0;
	virtual const bool vProcessMessage( const InputMessage& message ) = 0;
protected:
	std::shared_ptr< GameObject >	_owner;
};