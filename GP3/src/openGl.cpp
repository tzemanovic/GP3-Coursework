#include "pch.h"
#include "openGl.h"
#include "3rdParty/glext.h"
#include "3rdParty/wglext.h"
#include "renderable.h"

OpenGl::OpenGl( OpenGlConfig& openGlConfig, WindowHandle window, unsigned bitsPerPx ) : _openGlConfig( openGlConfig ), _window( window ), _deviceContext( nullptr ), _openGlContext( nullptr )
{
	// get handle to a device context
	_deviceContext = GetDC( _window );
	// first we need to create OpenGL context and make it current to be able to use wglGetProcAddress function
	if ( _deviceContext != NULL )
	{
		// get closest pixel format descriptor match
		PIXELFORMATDESCRIPTOR desc;
		ZeroMemory( &desc, sizeof( desc ) );
		desc.nSize = sizeof( desc );
		desc.nVersion = 1;
		desc.iLayerType = PFD_MAIN_PLANE;
		desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		desc.iPixelType = PFD_TYPE_RGBA;
		desc.cColorBits = static_cast< BYTE >( bitsPerPx );
		desc.cDepthBits = static_cast< BYTE >( _openGlConfig.depthBits );
		desc.cStencilBits = static_cast< BYTE >( _openGlConfig.stencilBits );
		desc.cAlphaBits = bitsPerPx == 32 ? 8 : 0;
		int iPixelFormat = ChoosePixelFormat( _deviceContext, &desc );
		// get descriptor for the match
		PIXELFORMATDESCRIPTOR actualDesc;
		actualDesc.nSize = sizeof( actualDesc );
		actualDesc.nVersion = 1;
		DescribePixelFormat( _deviceContext, iPixelFormat, sizeof( actualDesc ), &actualDesc );
		_openGlConfig.depthBits = actualDesc.cDepthBits;
		_openGlConfig.stencilBits = actualDesc.cStencilBits;
		// set pixel format of the device context
		if ( SetPixelFormat( _deviceContext, iPixelFormat, &actualDesc ) == TRUE )
		{
			// we need to create a context first to be able to get WGL functions
			_openGlContext = wglCreateContext( _deviceContext );
			// make it the calling thread's current rendering context 
			if ( wglMakeCurrent( _deviceContext, _openGlContext ) == TRUE )
			{
				glewInit( );
				createContext( bitsPerPx );
			}
		}
	}
}
OpenGl::~OpenGl( )
{
	if ( _openGlContext != nullptr )
	{
		// make the rendering context not current 
		wglMakeCurrent( NULL, NULL );
		// delete the rendering context  
		wglDeleteContext( _openGlContext );
		_openGlContext = nullptr;
	}
	if ( _deviceContext != nullptr )
	{
		// release device context
		ReleaseDC( _window, _deviceContext );
		_deviceContext = nullptr;
	}
}
void OpenGl::createContext( unsigned bitsPerPx )
{
	// create a rendering context  
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = reinterpret_cast< PFNWGLCREATECONTEXTATTRIBSARBPROC >( wglGetProcAddress( "wglCreateContextAttribsARB" ) );
	if ( wglCreateContextAttribsARB )
	{
		// get OpenGL version 
		const char* ver = reinterpret_cast< const char* >( glGetString( GL_VERSION ) );
		_openGlConfig.majorVersion = ver[0] - '0';
		// if the major version is 3 or higher we can use glGetIntegerv instead 
		if ( _openGlConfig.majorVersion >= 3 )
		{
			glGetIntegerv( GL_MAJOR_VERSION, &_openGlConfig.majorVersion );
			glGetIntegerv( GL_MINOR_VERSION, &_openGlConfig.minorVersion );
		}
		else
		{
			_openGlConfig.minorVersion = ver[2] - '0';
		}
		int attribList[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, static_cast< int >( _openGlConfig.majorVersion ),
			WGL_CONTEXT_MINOR_VERSION_ARB, static_cast< int >( _openGlConfig.minorVersion ),
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0, 0
		};
		// delete the legacy context
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( _openGlContext );
		// create a new context using wglCreateContextAttribsARB instead of legacy function wglCreateContext
		_openGlContext = wglCreateContextAttribsARB( _deviceContext, 0, attribList );
		wglMakeCurrent( _deviceContext, _openGlContext );
	}
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
				_openGlConfig.antialiasingLevel--;
				piAttribIList[13] = _openGlConfig.antialiasingLevel;
				success = wglChoosePixelFormatARB( _deviceContext, piAttribIList, NULL, nMaxFormats, piFormats, &nNumFormats );
			}
			// if successful select the best available format from the list
			if ( success && nNumFormats > 0 )
			{
				PIXELFORMATDESCRIPTOR bestDesc;
				int iPixelFormat = 0;
				unsigned bestMatch = UINT_MAX;
				for ( UINT i = 0; i < nNumFormats; ++i )
				{
					// get descriptor for format		
					PIXELFORMATDESCRIPTOR desc;
					desc.nSize = sizeof( desc );
					desc.nVersion = 1;
					DescribePixelFormat( _deviceContext, piFormats[i], sizeof( desc ), &desc );
					// check how close it is to desired configuration
					unsigned match = std::abs( static_cast< int >( desc.cRedBits + desc.cGreenBits + desc.cBlueBits + desc.cAlphaBits - bitsPerPx ) )
						+ std::abs( static_cast< int >( _openGlConfig.depthBits - desc.cDepthBits ) )
						+ std::abs( static_cast< int >( _openGlConfig.stencilBits - desc.cStencilBits ) );
					// if it's closer to desired configuration than current best match, then this is the best match
					if ( match < bestMatch )
					{
						bestDesc = desc;
						iPixelFormat = piFormats[i];
						bestMatch = match;
					}
				}
				// set the pixel format with the best selected descriptor
				SetPixelFormat( _deviceContext, iPixelFormat, &bestDesc );
			}
		}
	}
}
void OpenGl::display( )
{
	if ( _deviceContext != nullptr && _openGlContext != nullptr )
	{
		SwapBuffers( _deviceContext );
	}
}
void OpenGl::clear( float red, float green, float blue, float alpha )
{
	glClearColor( red, green, blue, alpha );
	glClearDepth( 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}