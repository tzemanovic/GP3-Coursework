#pragma once

#include "openGlConfig.h"

class OpenGl
{
public:
	OpenGl( OpenGlConfig& openGlConfig, WindowHandle window, unsigned bitsPerPx );
	~OpenGl( );
	void createContext( unsigned bitsPerPx );
private:
	OpenGlConfig&		_openGlConfig;
	WindowHandle		_window;
	DeviceContextHandle	_deviceContext;
	OpenGlContextHandle _openGlContext;
};