#ifndef OPEN_GL_CONFIG_H
#define OPEN_GL_CONFIG_H

struct OpenGlConfig
{
public:
	OpenGlConfig( ) : antialiasingLevel( 8 ), depthBits( 0 ), stencilBits( 0 )
	{
	}
	OpenGlConfig( unsigned antialiasingLevel ) : antialiasingLevel( antialiasingLevel ), depthBits( 0 ), stencilBits( 0 )
	{
	}
public:
	void decreaseAntialiasingLevel( )
	{
		antialiasingLevel--;
	}
public:
	unsigned antialiasingLevel;
	unsigned depthBits;
	unsigned stencilBits;
};

#endif