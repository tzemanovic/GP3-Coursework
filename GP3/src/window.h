#pragma once

#include "windowConfig.h"
#include "inputMessage.h"
#include "openGlConfig.h"

class OpenGl;
class Renderable;
class Camera;
struct Time;

class Window
{
public:
	Window( String&& windowName, const WindowConfig& windowConfig, OpenGlConfig& openGlConfig );
	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;
public:
	bool isOpen( ) const;
	OpenGl* getOpenGl( ) const;
public:
	void close( );
	void switchToFullscreen( );
	bool popMessage( InputMessage& message );
	void display( );
	void clear( float red, float green, float blue, float alpha = 0.f );
public:
	const unsigned getWidth( ) const
	{
		return _width;
	}
	const unsigned getHeight( ) const
	{
		return _height;
	}
public:
	static std::tuple<unsigned, unsigned, unsigned> getCurrentMode( );
	static double time( );
private:
	std::queue<InputMessage>	_messages;
	std::array<bool, 255>		_oldKeyStates;
	WindowHandle				_windowHandle;
	OpenGl*						_openGl;
	HCURSOR						_mouseCursor;
	unsigned					_width;
	unsigned					_height;
	unsigned					_bitsPerPx;
	WindowStyle					_windowStyle;
	bool						_open;
	bool						_mouseCaptured;
	LPCWSTR 					_windowName;
private:
	LRESULT CALLBACK wndProc( WindowHandle handle, UINT message, WPARAM wParam, LPARAM lParam );
	void processMessages( );
	void setMouseCursorVisibility( bool visible );
	void captureMouse( );
	void releaseMouse( );
	bool isOnlyInstance( );
private:
	static void setWindowVisibility( WindowHandle windowHandle, bool visible );
	static LRESULT CALLBACK staticWndProc( WindowHandle handle, UINT message, WPARAM wParam, LPARAM lParam );
};