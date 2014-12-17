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
}