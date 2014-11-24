#ifndef WINDOW_H
#define WINDOW_H

#include "windowConfig.h"
#include "inputMessage.h"

typedef HWND WindowHandle;

class Window
{
public:
	Window( const wchar_t* windowName, const WindowConfig& windowConfig );
	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;
public:
	bool isOpen( ) const;
public:
	void close( );
	void switchToFullscreen( );
	bool popMessage( InputMessage& message );
public:
	static std::tuple<unsigned, unsigned, unsigned> getCurrentMode( );
	static double time( );
private:
	std::queue<InputMessage>	_messages;
	std::array<bool, 255>		_oldKeyStates;
	WindowHandle				_windowHandle;
	HCURSOR						_mouseCursor;
	unsigned					_width;
	unsigned					_height;
	unsigned					_bitsPerPx;
	WindowStyle					_windowStyle;
	bool						_open;
	bool						_mouseCaptured;
	const wchar_t*				_windowName;
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

#endif