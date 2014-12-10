#pragma once

class InputMessage;
class OpenGl;
struct Time;

class View
{
public:
	View( );
	virtual ~View( );
	View( const View& ) = delete;
	View& operator=( const View& ) = delete;
public:
	virtual void vRender( const Time& time, OpenGl* openGl ) = 0;
	virtual void vUpdate( const Time& time ) = 0;
	virtual const bool vProcessMessage( const InputMessage& message ) = 0;
protected:
};