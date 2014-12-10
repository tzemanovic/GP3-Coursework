#pragma once

class OpenGl;
class Camera;
struct Time;

class Renderable
{
public:
	Renderable( )
	{
	}
	virtual ~Renderable( )
	{
	}
public:
	virtual void vRender( const Time& time, OpenGl* openGl, std::shared_ptr< Camera > camera ) = 0;
};