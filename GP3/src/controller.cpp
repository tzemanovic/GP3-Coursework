#include "pch.h"
#include "controller.h"
#include "inputMessage.h"

Controller::Controller( )
{
	// initialize arrays to false
	memset( _keyState, false, sizeof( _keyState ) );
	memset( _keyOldState, false, sizeof( _keyOldState ) );
}
const bool Controller::processMessage( const InputMessage& message )
{
	// store key and mouse states
	if ( message == InputMessage::Type::KeyDown )
	{
		_keyState[+message.key.type] = true;
		return true;
	}
	else if ( message == InputMessage::Type::KeyUp )
	{
		_keyState[+message.key.type] = false;
		return true;
	}
	else if ( message == InputMessage::Type::MouseMove )
	{
		_mouseX = message.mouseMove.x;
		_mouseY = message.mouseMove.y;
		return true;
	}
	return false;
}
void Controller::update( const Time& time )
{
	if ( _onUpdate != nullptr )
	{
		_onUpdate( *this, _controlledObject, time );
	}
	// keep copy of key states for next frame to be able get around the fact that KeyDown event usually repeats multiple times
	memcpy( _keyOldState, _keyState, sizeof( _keyOldState ) );
}