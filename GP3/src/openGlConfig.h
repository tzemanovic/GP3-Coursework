#pragma once

struct OpenGlConfig
{
public:
	OpenGlConfig( ) : antialiasingLevel( 16 ), depthBits( 0 ), stencilBits( 0 ), majorVersion( 0 ), minorVersion( 0 )
	{
	}
	OpenGlConfig( unsigned antialiasingLevel ) : antialiasingLevel( antialiasingLevel ), depthBits( 0 ), stencilBits( 0 ), majorVersion( 0 ), minorVersion( 0 )
	{
	}
public:
	unsigned antialiasingLevel;
	unsigned depthBits;
	unsigned stencilBits;
	int majorVersion;
	int minorVersion;
};