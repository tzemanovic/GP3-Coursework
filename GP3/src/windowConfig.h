#pragma once

enum class WindowStyle : unsigned
{
	Windowed,
	Fullscreen,
	Default = Windowed
};

class WindowConfig
{
public:
	WindowConfig( );
	explicit WindowConfig( WindowStyle windowStyle );
	WindowConfig( const WindowConfig& ) = delete;
	const WindowConfig& operator=( const WindowConfig& ) = delete;
	WindowConfig( WindowConfig&& ) = delete;
	const WindowConfig& operator=( WindowConfig&& ) = delete;
public:
	const unsigned getWidth( ) const
	{
		return _width;
	}
	void setWidth( const unsigned width )
	{
		_width = width;
	}
	const unsigned getHeight( ) const
	{
		return _height;
	}
	void setHeight( const unsigned height )
	{
		_height = height;
	}
	const unsigned getBitsPerPx( ) const
	{
		return _bitsPerPx;
	}
	const WindowStyle getWindowStyle( ) const
	{
		return _windowStyle;
	}
	void setWindowStyle( const WindowStyle style )
	{
		_windowStyle = style;
	}
private:
	unsigned		_width;
	unsigned		_height;
	unsigned		_bitsPerPx;
	WindowStyle		_windowStyle;
};