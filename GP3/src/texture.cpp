#include "pch.h"
#include "texture.h"
#include "stringUtils.h"
#include "openGl.h"

Texture::Texture( const GLenum textureTarget, const String& filename ) : _textureTarget( textureTarget ), _filename( filename )
{
	const char* filenameC = _filename.c_str( );
	FREE_IMAGE_FORMAT format = FIF_UNKNOWN;
	format = FreeImage_GetFileType( filenameC, 0 );
	if ( format == FIF_UNKNOWN )
	{
		// guess the file format from the file extension if unknown
		format = FreeImage_GetFIFFromFilename( filenameC );
	}
	if ( format == FIF_UNKNOWN )
	{
		ERR( "Unable to load texture image " << _filename << ". Unrecognized file format" << std::endl );
	}
	else
	{
		FIBITMAP* image = nullptr;
		if ( FreeImage_FIFSupportsReading( format ) )
		{
			image = FreeImage_Load( format, filenameC );
		}
		else
		{
			ERR( "Unable to load texture image " << _filename << ". Unsupported file format" << std::endl );
		}
		if ( image == nullptr )
		{
			ERR( "Unable to load texture image " << _filename << "." << std::endl );
		}
		else
		{
			BYTE* data = FreeImage_GetBits( image );
			int width, height, bpp, internalFormat, format;
			width = FreeImage_GetWidth( image );
			height = FreeImage_GetHeight( image );
			bpp = FreeImage_GetBPP( image );
			switch ( bpp )
			{
			case 24:
				internalFormat = GL_RGB;
				format = GL_BGR;
				break;
			default:
				internalFormat = GL_RGBA;
				format = GL_BGRA;
				break;
			}			
			// generate texture name
			glGenTextures( 1, &_texture );
			// bind named texture to texturing target
			glBindTexture( _textureTarget, _texture );
			// specify texture
			glTexImage2D( _textureTarget, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data );
			glGenerateMipmap( _textureTarget );
			FreeImage_Unload( image );
			glGenSamplers( 1, &_sampler );
			// use linear interpolation for magnification filter
			glSamplerParameteri( _sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			// use linear interpolation for minifying filter
			glSamplerParameteri( _sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glErrorCheck( );
		}
	}
}
Texture::~Texture( )
{
	glDeleteTextures( 1, &_texture );
}
void Texture::bind( const GLint textureUnit )
{
	// select active texture unit
	glActiveTexture( GL_TEXTURE0 + textureUnit );
	// bind named texture to texturing target
	glBindTexture( _textureTarget, _texture );
	// bind named sampler to texturing target
	glBindSampler( textureUnit, _sampler );
	glErrorCheck( );
}