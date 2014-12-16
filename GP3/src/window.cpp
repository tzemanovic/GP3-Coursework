#include "pch.h"
#include "window.h"
#include "openGl.h"
#include "renderable.h"
#include "stringUtils.h"

Window::Window( String&& windowName, const WindowConfig& windowConfig, OpenGlConfig& openGlConfig ) : _oldKeyStates( ), _windowHandle( nullptr ), _openGl( nullptr ), _mouseCursor( nullptr ),
_width( windowConfig.getWidth( ) ), _height( windowConfig.getHeight( ) ), _bitsPerPx( windowConfig.getBitsPerPx( ) ), _windowStyle( windowConfig.getWindowStyle( ) ), _open( false ), _mouseCaptured( false )
{
	std::wstring windowNameW = s2ws( windowName );
	_windowName = windowNameW.c_str( );
	if ( isOnlyInstance( ) )
	{
		// window class details
		WNDCLASSEX windowClass = { 0 };
		windowClass.cbSize = sizeof( WNDCLASSEX );
		windowClass.style = CS_VREDRAW | CS_HREDRAW;
		windowClass.lpfnWndProc = &staticWndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetModuleHandle( nullptr );
		windowClass.hIcon = 0;
		windowClass.hIconSm = 0;
		windowClass.hCursor = 0;
		windowClass.hbrBackground = 0;
		windowClass.lpszMenuName = 0;
		windowClass.lpszClassName = _windowName;
		// register the window
		if ( RegisterClassEx( &windowClass ) )
		{
			// find position and size
			HDC screenDC = GetDC( nullptr );
			unsigned left = ( GetDeviceCaps( screenDC, HORZRES ) - _width ) / 2;
			unsigned top = ( GetDeviceCaps( screenDC, VERTRES ) - _height ) / 2;
			unsigned width = _width;
			unsigned height = _height;
			ReleaseDC( nullptr, screenDC );
			// set the style of the window
			DWORD style = WS_VISIBLE;
			if ( _windowStyle == WindowStyle::Windowed )
			{
				style |= WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_SYSMENU;
				// adjust the window size with the borders etc.
				RECT rectangle = { 0, 0, _width, _height };
				AdjustWindowRect( &rectangle, style, false );
				width = rectangle.right - rectangle.left;
				height = rectangle.bottom - rectangle.top;
			}
			// create the window
			_windowHandle = CreateWindowEx( 0, _windowName, _windowName, style, left, top, width, height, GetDesktopWindow( ), nullptr, GetModuleHandle( nullptr ), this );
			_open = true;
			if ( _windowStyle == WindowStyle::Fullscreen )
			{
				switchToFullscreen( );
			}
			_openGl = new OpenGl( openGlConfig, _windowHandle, windowConfig.getBitsPerPx( ) );
		}
	}
}
bool Window::isOpen( ) const
{
	return _open;
}
OpenGl* Window::getOpenGl( ) const
{
	return _openGl;
}
void Window::close( )
{
	// destroy OpenGL context
	delete( _openGl );
	_openGl = nullptr;
	// show cursor in case it was hidden
	setMouseCursorVisibility( true );
	ChangeDisplaySettings( 0, 0 );
	if ( _windowHandle )
	{
		DestroyWindow( _windowHandle );
	}
	// unregister window class
	UnregisterClass( _windowName, GetModuleHandle( nullptr ) );
	_open = false;
}
void Window::switchToFullscreen( )
{
	// set display settings
	DEVMODE devMode;
	devMode.dmSize = sizeof( devMode );
	devMode.dmPelsWidth = _width;
	devMode.dmPelsHeight = _height;
	devMode.dmBitsPerPel = _bitsPerPx;
	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
	// change default display device settings
	if ( ChangeDisplaySettings( &devMode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
	{
		return;
	}
	// set window style
	SetWindowLong( _windowHandle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS );
	// set extended window style
	SetWindowLong( _windowHandle, GWL_EXSTYLE, WS_EX_APPWINDOW );
	// set window size, position and z-order
	SetWindowPos( _windowHandle, HWND_TOP, 0, 0, _width, _height, SWP_FRAMECHANGED );
	// show the window
	setWindowVisibility( _windowHandle, true );
}
bool Window::popMessage( InputMessage& message )
{
	// if there are no messages in the queue, check for new ones
	if ( _messages.empty( ) )
	{
		processMessages( );
	}
	// if there are messages, pop a next message
	if ( !_messages.empty( ) )
	{
		message = _messages.front( );
		_messages.pop( );
		return true;
	}
	return false;
}
void Window::render( const Time& time, Renderable* renderable, std::shared_ptr< Camera > camera )
{
	renderable->vRender( time, camera );
}
void Window::display( )
{
	_openGl->display( );
}
void Window::clear( float red, float green, float blue, float alpha )
{
	_openGl->clear( red, green, blue, alpha );
}
LRESULT CALLBACK Window::wndProc( WindowHandle handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
	case WM_CLOSE:
	{
		// create close message and push it to the queue
		InputMessage msg( InputMessage::Type::Close );
		_messages.push( msg );
		break;
	}
	case WM_ACTIVATEAPP:
	{
		if ( wParam && _mouseCaptured )
		{
			captureMouse( );
		}
		break;
	}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		if ( !_oldKeyStates[wParam] )
		{
			InputMessage msg( InputMessage::Type::KeyDown );
			msg.key.type = ( Key ) wParam;
			msg.key.lCtrl = HIWORD( GetKeyState( VK_RCONTROL ) ) != 0;
			msg.key.rCtrl = HIWORD( GetKeyState( VK_RCONTROL ) ) != 0;
			msg.key.lAlt = HIWORD( GetKeyState( VK_LMENU ) ) != 0;
			msg.key.rAlt = HIWORD( GetKeyState( VK_RMENU ) ) != 0;
			msg.key.lShift = HIWORD( GetKeyState( VK_LSHIFT ) ) != 0;
			msg.key.rShift = HIWORD( GetKeyState( VK_RSHIFT ) ) != 0;
			_messages.push( msg );
			_oldKeyStates[wParam] = true;
		}
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		InputMessage msg( InputMessage::Type::KeyUp );
		msg.key.type = ( Key ) wParam;
		msg.key.lCtrl = HIWORD( GetKeyState( VK_RCONTROL ) ) != 0;
		msg.key.rCtrl = HIWORD( GetKeyState( VK_RCONTROL ) ) != 0;
		msg.key.lAlt = HIWORD( GetKeyState( VK_LMENU ) ) != 0;
		msg.key.rAlt = HIWORD( GetKeyState( VK_RMENU ) ) != 0;
		msg.key.lShift = HIWORD( GetKeyState( VK_LSHIFT ) ) != 0;
		msg.key.rShift = HIWORD( GetKeyState( VK_RSHIFT ) ) != 0;
		_messages.push( msg );
		_oldKeyStates[wParam] = false;
		break;
	}
	case WM_CHAR:
	{
		InputMessage msg( InputMessage::Type::Text );
		msg.text.unicode = wParam;
		_messages.push( msg );
		break;
	}
	case WM_MOUSEMOVE:
	{
		InputMessage msg( InputMessage::Type::MouseMove );
		msg.mouseMove.x = LOWORD( lParam );
		msg.mouseMove.y = HIWORD( lParam );
		msg.mouseMove.ctrl = ( wParam & MK_CONTROL ) != 0;
		msg.mouseMove.lButton = ( wParam & MK_LBUTTON ) != 0;
		msg.mouseMove.mButton = ( wParam & MK_MBUTTON ) != 0;
		msg.mouseMove.rButton = ( wParam & MK_RBUTTON ) != 0;
		msg.mouseMove.shift = ( wParam & MK_SHIFT ) != 0;
		msg.mouseMove.xButton1 = ( wParam & MK_XBUTTON1 ) != 0;
		msg.mouseMove.xButton2 = ( wParam & MK_XBUTTON2 ) != 0;
		_messages.push( msg );
		break;
	}
	case WM_LBUTTONDOWN:
	{
		InputMessage msg( InputMessage::Type::MouseButtonDown );
		msg.mouseButton.type = MouseButton::LeftButton;
		msg.mouseButton.ctrl = ( wParam & MK_CONTROL ) != 0;
		msg.mouseButton.lButton = ( wParam & MK_LBUTTON ) != 0;
		msg.mouseButton.mButton = ( wParam & MK_MBUTTON ) != 0;
		msg.mouseButton.rButton = ( wParam & MK_RBUTTON ) != 0;
		msg.mouseButton.shift = ( wParam & MK_SHIFT ) != 0;
		msg.mouseButton.xButton1 = ( wParam & MK_XBUTTON1 ) != 0;
		msg.mouseButton.xButton2 = ( wParam & MK_XBUTTON2 ) != 0;
		msg.mouseButton.x = LOWORD( lParam );
		msg.mouseButton.y = HIWORD( lParam );
		_messages.push( msg );
		captureMouse( );
		break;
	}
	case WM_LBUTTONUP:
	{
		InputMessage msg( InputMessage::Type::MouseButtonUp );
		msg.mouseButton.type = MouseButton::LeftButton;
		msg.mouseButton.x = LOWORD( lParam );
		msg.mouseButton.y = HIWORD( lParam );
		_messages.push( msg );
		releaseMouse( );
		break;
	}
	case WM_RBUTTONDOWN:
	{
		InputMessage msg( InputMessage::Type::MouseButtonDown );
		msg.mouseButton.type = MouseButton::RightButton;
		msg.mouseButton.ctrl = ( wParam & MK_CONTROL ) != 0;
		msg.mouseButton.lButton = ( wParam & MK_LBUTTON ) != 0;
		msg.mouseButton.mButton = ( wParam & MK_MBUTTON ) != 0;
		msg.mouseButton.rButton = ( wParam & MK_RBUTTON ) != 0;
		msg.mouseButton.shift = ( wParam & MK_SHIFT ) != 0;
		msg.mouseButton.xButton1 = ( wParam & MK_XBUTTON1 ) != 0;
		msg.mouseButton.xButton2 = ( wParam & MK_XBUTTON2 ) != 0;
		msg.mouseButton.x = LOWORD( lParam );
		msg.mouseButton.y = HIWORD( lParam );
		_messages.push( msg );
		captureMouse( );
		break;
	}
	case WM_RBUTTONUP:
	{
		InputMessage msg( InputMessage::Type::MouseButtonUp );
		msg.mouseButton.type = MouseButton::RightButton;
		msg.mouseButton.x = LOWORD( lParam );
		msg.mouseButton.y = HIWORD( lParam );
		_messages.push( msg );
		releaseMouse( );
		break;
	}
	case WM_MBUTTONDOWN:
	{
		InputMessage msg( InputMessage::Type::MouseButtonDown );
		msg.mouseButton.type = MouseButton::MiddleButton;
		msg.mouseButton.ctrl = ( wParam & MK_CONTROL ) != 0;
		msg.mouseButton.lButton = ( wParam & MK_LBUTTON ) != 0;
		msg.mouseButton.mButton = ( wParam & MK_MBUTTON ) != 0;
		msg.mouseButton.rButton = ( wParam & MK_RBUTTON ) != 0;
		msg.mouseButton.shift = ( wParam & MK_SHIFT ) != 0;
		msg.mouseButton.xButton1 = ( wParam & MK_XBUTTON1 ) != 0;
		msg.mouseButton.xButton2 = ( wParam & MK_XBUTTON2 ) != 0;
		msg.mouseButton.x = LOWORD( lParam );
		msg.mouseButton.y = HIWORD( lParam );
		_messages.push( msg );
		captureMouse( );
		break;
	}
	case WM_MBUTTONUP:
	{
		InputMessage msg( InputMessage::Type::MouseButtonUp );
		msg.mouseButton.type = MouseButton::MiddleButton;
		msg.mouseButton.x = LOWORD( lParam );
		msg.mouseButton.y = HIWORD( lParam );
		_messages.push( msg );
		releaseMouse( );
		break;
	}
	case WM_MOUSEWHEEL:
	{
		POINT screenPos;
		screenPos.x = static_cast< LONG >( LOWORD( lParam ) );
		screenPos.y = static_cast< LONG >( HIWORD( lParam ) );
		ScreenToClient( handle, &screenPos );
		InputMessage msg( InputMessage::Type::MouseWheel );
		msg.mouseWheel.ctrl = ( wParam & MK_CONTROL ) != 0;
		msg.mouseWheel.lButton = ( wParam & MK_LBUTTON ) != 0;
		msg.mouseWheel.mButton = ( wParam & MK_MBUTTON ) != 0;
		msg.mouseWheel.rButton = ( wParam & MK_RBUTTON ) != 0;
		msg.mouseWheel.shift = ( wParam & MK_SHIFT ) != 0;
		msg.mouseWheel.xButton1 = ( wParam & MK_XBUTTON1 ) != 0;
		msg.mouseWheel.xButton2 = ( wParam & MK_XBUTTON2 ) != 0;
		msg.mouseWheel.x = static_cast< uint16_t >( screenPos.x );
		msg.mouseWheel.y = static_cast< uint16_t >( screenPos.y );
		msg.mouseWheel.delta = HIWORD( wParam ) / 120;
		_messages.push( msg );
		break;
	}
	case WM_XBUTTONDOWN:
	{
		InputMessage msg( InputMessage::Type::MouseButtonDown );
		msg.mouseButton.type = HIWORD( wParam ) == XBUTTON1 ? MouseButton::XButton1 : MouseButton::XButton2;
		msg.mouseButton.ctrl = ( wParam & MK_CONTROL ) != 0;
		msg.mouseButton.lButton = ( wParam & MK_LBUTTON ) != 0;
		msg.mouseButton.mButton = ( wParam & MK_MBUTTON ) != 0;
		msg.mouseButton.rButton = ( wParam & MK_RBUTTON ) != 0;
		msg.mouseButton.shift = ( wParam & MK_SHIFT ) != 0;
		msg.mouseButton.xButton1 = ( wParam & MK_XBUTTON1 ) != 0;
		msg.mouseButton.xButton2 = ( wParam & MK_XBUTTON2 ) != 0;
		msg.mouseButton.x = LOWORD( lParam );
		msg.mouseButton.y = HIWORD( lParam );
		_messages.push( msg );
		break;
	}
	case WM_XBUTTONUP:
	{
		InputMessage msg( InputMessage::Type::MouseButtonUp );
		msg.mouseButton.type = HIWORD( wParam ) == XBUTTON1 ? MouseButton::XButton1 : MouseButton::XButton2;
		msg.mouseButton.x = LOWORD( lParam );
		msg.mouseButton.y = HIWORD( lParam );
		_messages.push( msg );
		break;
	}
	default:
		// default window procedure
		return DefWindowProc( handle, message, wParam, lParam );
	}
	return 0;
}
void Window::processMessages( )
{
	MSG message;
	while ( PeekMessage( &message, _windowHandle, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &message );
		DispatchMessage( &message );
	}
}
void Window::setMouseCursorVisibility( bool visible )
{
	if ( visible )
	{
		if ( !_mouseCursor )
			_mouseCursor = LoadCursor( NULL, IDC_ARROW );
		SetCursor( _mouseCursor );
	}
	else
	{
		SetCursor( NULL );
	}
}
void Window::captureMouse( )
{
	SetCapture( _windowHandle );
	_mouseCaptured = true;
}
void Window::releaseMouse( )
{
	ReleaseCapture( );
	_mouseCaptured = false;
}
void Window::setWindowVisibility( WindowHandle windowHandle, bool visible )
{
	ShowWindow( windowHandle, visible ? SW_SHOW : SW_HIDE );
	if ( visible )
	{
		SetFocus( windowHandle );
		SetForegroundWindow( windowHandle );
		SetActiveWindow( windowHandle );
	}
}
bool Window::isOnlyInstance( )
{
	HANDLE handle = CreateMutex( nullptr, true, _windowName );
	if ( GetLastError( ) != ERROR_SUCCESS )
	{
		WindowHandle windowHandle = FindWindow( _windowName, nullptr );
		if ( windowHandle != nullptr )
		{
			setWindowVisibility( windowHandle, true );
			return false;
		}
	}
	return true;
}
LRESULT CALLBACK Window::staticWndProc( WindowHandle handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	Window* window;
	if ( message == WM_NCCREATE )
	{
		CREATESTRUCT *cs = ( CREATESTRUCT* ) lParam;
		window = ( Window* ) cs->lpCreateParams;
		SetLastError( 0 );
		if ( SetWindowLongPtr( handle, GWL_USERDATA, ( LONG_PTR ) window ) == 0 )
		{
			if ( GetLastError( ) != 0 )
				return FALSE;
		}
	}
	else
	{
		window = ( Window* ) GetWindowLongPtr( handle, GWL_USERDATA );
	}
	if ( window )
	{
		return window->wndProc( handle, message, wParam, lParam );
	}
	return DefWindowProc( handle, message, wParam, lParam );
}
std::tuple<unsigned, unsigned, unsigned> Window::getCurrentMode( )
{
	DEVMODE mode;
	mode.dmSize = sizeof( mode );
	EnumDisplaySettings( nullptr, ENUM_CURRENT_SETTINGS, &mode );
	return std::make_tuple( mode.dmPelsWidth, mode.dmPelsHeight, mode.dmBitsPerPel );
}
double Window::time( )
{
	// use only single thread to calculate time
	HANDLE currentThread = GetCurrentThread( );
	DWORD_PTR previousMask = SetThreadAffinityMask( currentThread, 1 );
	static LARGE_INTEGER frequency;
	QueryPerformanceFrequency( &frequency );
	LARGE_INTEGER time;
	QueryPerformanceCounter( &time );
	// use previously used thread
	SetThreadAffinityMask( currentThread, previousMask );
	return 1000 * static_cast< double >( time.QuadPart ) / static_cast< double >( frequency.QuadPart );
}
