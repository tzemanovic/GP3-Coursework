#include "pch.h"
#include "WindowConfig.h"
#include "Window.h"

WindowConfig::WindowConfig( ) : _windowStyle( WindowStyle::Default )
{
	std::tie( _width, _height, _bitsPerPx ) = Window::getCurrentMode( );
}
WindowConfig::WindowConfig( WindowStyle windowStyle ) : _windowStyle( windowStyle )
{
	std::tie( _width, _height, _bitsPerPx ) = Window::getCurrentMode( );
}