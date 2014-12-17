#pragma once

#include "inputMessage.h"

class GameObject;
struct Time;

class Controller
{
	typedef std::function< void( const Controller&, std::shared_ptr< GameObject >, const Time& ) > OnUpdate;
public:
	Controller( );
public:
	const bool processMessage( const InputMessage& message );
	void update( const Time& time );
public:
	void setControlledObject( std::shared_ptr< GameObject > controlledObject )
	{
		_controlledObject = controlledObject;
	}
	void setOnUpdate( OnUpdate onUpdate )
	{
		_onUpdate = onUpdate;
	}
	bool getKeyState( Key key ) const
	{
		return _keyState[+key];
	}
	bool getOldKeyState( Key key ) const
	{
		return _keyOldState[+key];
	}
	unsigned short getMouseX( ) const
	{
		return _mouseX;
	}
	unsigned short getMouseY( ) const
	{
		return _mouseY;
	}
protected:
	std::shared_ptr< GameObject > 	_controlledObject;
	OnUpdate 						_onUpdate;
	bool 							_keyState[255];
	bool 							_keyOldState[255];
	unsigned short					_mouseX;
	unsigned short					_mouseY;
};