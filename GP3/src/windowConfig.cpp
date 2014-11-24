#include "pch.h"
#include "WindowConfig.h"
#include "Window.h"

WindowConfig::WindowConfig( ) : _windowStyle( WindowStyle::Default )
{
	std::tie( _width, _height, _bitsPerPx ) = Window::getCurrentMode( );
}
unsigned WindowConfig::getWidth( ) const
{
	return _width;
}
unsigned WindowConfig::getHeight( ) const
{
	return _height;
}
unsigned WindowConfig::getBitsPerPx( ) const
{
	return _bitsPerPx;
}
WindowStyle WindowConfig::getWindowStyle( ) const
{
	return _windowStyle;
}
