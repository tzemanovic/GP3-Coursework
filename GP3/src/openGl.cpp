#include "pch.h"
#include "openGl.h"
#include "wglext.h"

OpenGl::OpenGl( OpenGlConfig& openGlConfig, WindowHandle window, unsigned bitsPerPx ) : _openGlConfig( openGlConfig ), _window( window ), _deviceContext( nullptr ), _openGlContext( nullptr )
{
	// get handle to a device context
	_deviceContext = GetDC( _window );
	// first we need to create OpenGL context and make it current to be able to use wglGetProcAddress function
	if ( _deviceContext != NULL )
	{
		// get closest pixel format descriptor match
		PIXELFORMATDESCRIPTOR descriptor;
		ZeroMemory( &descriptor, sizeof( descriptor ) );
		descriptor.nSize = sizeof( descriptor );
		descriptor.nVersion = 1;
		descriptor.iLayerType = PFD_MAIN_PLANE;
		descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		descriptor.iPixelType = PFD_TYPE_RGBA;
		descriptor.cColorBits = static_cast< BYTE >( bitsPerPx );
		descriptor.cDepthBits = static_cast< BYTE >( _openGlConfig.depthBits );
		descriptor.cStencilBits = static_cast< BYTE >( _openGlConfig.stencilBits );
		descriptor.cAlphaBits = bitsPerPx == 32 ? 8 : 0;
		int iPixelFormat = ChoosePixelFormat( _deviceContext, &descriptor );
		// get descriptor for the match
		PIXELFORMATDESCRIPTOR actualFormat;
		actualFormat.nSize = sizeof( actualFormat );
		actualFormat.nVersion = 1;
		DescribePixelFormat( _deviceContext, iPixelFormat, sizeof( actualFormat ), &actualFormat );
		_openGlConfig.depthBits = actualFormat.cDepthBits;
		_openGlConfig.stencilBits = actualFormat.cStencilBits;
		// set pixel format of the device context
		if ( SetPixelFormat( _deviceContext, iPixelFormat, &actualFormat ) == TRUE )
		{
			// create a rendering context  
			_openGlContext = wglCreateContext( _deviceContext );
			// make it the calling thread's current rendering context 
			if ( wglMakeCurrent( _deviceContext, _openGlContext ) == TRUE )
			{
				createContext( bitsPerPx );
			}
		}
	}
}
OpenGl::~OpenGl( )
{
	// make the rendering context not current 
	wglMakeCurrent( NULL, NULL );
	// delete the rendering context  
	wglDeleteContext( _openGlContext );
}
void OpenGl::createContext( unsigned bitsPerPx )
{
	int format = 0;
	if ( _openGlConfig.antialiasingLevel > 0 )
	{
		// get wglext function address
		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = reinterpret_cast< PFNWGLCHOOSEPIXELFORMATARBPROC >( wglGetProcAddress( "wglChoosePixelFormatARB" ) );
		if ( wglChoosePixelFormatARB == NULL )
		{
			// disable antialiasing if not supported
			_openGlConfig.antialiasingLevel = 0;
		}
		else
		{
			// wgl attribute list
			int piAttribIList[] =
			{
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
				WGL_SAMPLES_ARB, static_cast< int >( _openGlConfig.antialiasingLevel ),
				0, 0
			};
			const static int nMaxFormats = 128;
			int piFormats[nMaxFormats];
			UINT nNumFormats;
			// try to get available formats
			BOOL success = wglChoosePixelFormatARB( _deviceContext, piAttribIList, NULL, nMaxFormats, piFormats, &nNumFormats );
			// if unsuccessful or no formats available, try to decrease antialiasing level
			while ( _openGlConfig.antialiasingLevel > 0 && ( success == FALSE || nNumFormats == 0 ) )
			{
				_openGlConfig.decreaseAntialiasingLevel( );
				piAttribIList[11] = _openGlConfig.antialiasingLevel;
				success = wglChoosePixelFormatARB( _deviceContext, piAttribIList, NULL, nMaxFormats, piFormats, &nNumFormats );
			}
			// if successful select the best available format from the list
			if ( success && nNumFormats > 0 )
			{
				unsigned bestMatch = UINT_MAX;
				for ( UINT i = 0; i < nNumFormats; ++i )
				{
					// get descriptor for format		
					PIXELFORMATDESCRIPTOR attributes;
					attributes.nSize = sizeof( attributes );
					attributes.nVersion = 1;
					DescribePixelFormat( _deviceContext, piFormats[i], sizeof( attributes ), &attributes );
					// check how close it is to desired configuration
					unsigned match = std::abs( static_cast< int >( attributes.cRedBits + attributes.cGreenBits + attributes.cBlueBits + attributes.cAlphaBits - bitsPerPx ) )
						+ std::abs( static_cast< int >( _openGlConfig.depthBits - attributes.cDepthBits ) )
						+ std::abs( static_cast< int >( _openGlConfig.stencilBits - attributes.cStencilBits ) );
					// if it's closer to desired configuration than current best match, then this is the best match
					if ( match < bestMatch )
					{
						format = piFormats[i];
						bestMatch = match;
					}
				}
			}
		}
	}
}