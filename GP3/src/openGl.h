#pragma once

#include "openGlConfig.h"

class OpenGl
{
public:
	OpenGl( OpenGlConfig& openGlConfig, WindowHandle window, unsigned bitsPerPx );
	~OpenGl( );
public:
	void createContext( unsigned bitsPerPx );
	void render( const double deltaMs );
	void clear( float red, float green, float blue, float alpha );
private:
	OpenGlConfig&		_openGlConfig;
	WindowHandle		_window;
	DeviceContextHandle	_deviceContext;
	OpenGlContextHandle _openGlContext;
};