#ifndef WINDOW_CONFIG_H
#define WINDOW_CONFIG_H

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
	unsigned getWidth( ) const;
	unsigned getHeight( ) const;
	unsigned getBitsPerPx( ) const;
	WindowStyle getWindowStyle( ) const;
private:
	unsigned		_width;
	unsigned		_height;
	unsigned		_bitsPerPx;
	WindowStyle		_windowStyle;
};

#endif