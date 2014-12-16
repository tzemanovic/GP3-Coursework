#pragma once

#include "openGlConfig.h"

class Renderable;
class Camera;
struct Time;

class OpenGl
{
public:
	OpenGl( OpenGlConfig& openGlConfig, WindowHandle window, unsigned bitsPerPx );
	virtual ~OpenGl( );
public:
	void display( );
	void clear( float red, float green, float blue, float alpha );
public:
	static void checkGlError( const char *file, int line );
	static void fillMode( );
	static void wireframeMode( );
	static void pointMode( );
private:
	void createContext( unsigned bitsPerPx );
private:
	OpenGlConfig&		_openGlConfig;
	WindowHandle		_window;
	DeviceContextHandle	_deviceContext;
	OpenGlContextHandle _openGlContext;
};

#ifdef _DEBUG
#define glErrorCheck() OpenGl::checkGlError(__FILE__,__LINE__)
#else
#define glErrorCheck()
#endif