#pragma once

#include "openGlConfig.h"

class Renderable;
class Camera;

class OpenGl
{
public:
	OpenGl( OpenGlConfig& openGlConfig, WindowHandle window, unsigned bitsPerPx );
	virtual ~OpenGl( );
public:
	void createContext( unsigned bitsPerPx );
	void display( );
	void clear( float red, float green, float blue, float alpha );
public:
private:
	OpenGlConfig&		_openGlConfig;
	WindowHandle		_window;
	DeviceContextHandle	_deviceContext;
	OpenGlContextHandle _openGlContext;
};