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


	/// old code using DevIL lib
	//ILuint image = ilGenImage( );
	//ilBindImage( image );
	//if ( ilLoadImage( s2ws( _filename ).c_str( ) ) ) )
	//{
	//	if ( ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE ) )
	//	{
	//		// generate texture name
	//		glGenTextures( 1, &_texture );
	//		// bind named texture to texturing target
	//		glBindTexture( _textureTarget, _texture );
	//		// use linear interpolation for magnification filter
	//		glTexParameteri( _textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//		// use linear interpolation for minifying filter
	//		glTexParameteri( _textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//		glTexImage2D(
	//			_textureTarget, 					// target texture
	//			0, 									// level-of-detail
	//			ilGetInteger( IL_IMAGE_BPP ), 		// internal format
	//			ilGetInteger( IL_IMAGE_WIDTH ), 	// image width
	//			ilGetInteger( IL_IMAGE_HEIGHT ), 	// image height
	//			0, 									// border
	//			ilGetInteger( IL_IMAGE_FORMAT ), 	// format of pixel data
	//			GL_UNSIGNED_BYTE, 					// type of pixel data
	//			ilGetData( ) );						// data
	//		// unbind texture
	//		//glBindTexture( _textureTarget, 0 );
	//		ilDeleteImage( image );
	//	}
	//	else
	//	{
	//		switch ( ilGetError( ) )
	//		{
	//		case IL_ILLEGAL_OPERATION:
	//			ERR( "Unable to convert texture image " << _filename << ". There is currently no image bound." << std::endl );
	//			break;
	//		case IL_INVALID_CONVERSION:
	//			ERR( "Unable to convert texture image " << _filename << ". DestFormat and/or DestType was an invalid identifier." << std::endl );
	//			break;
	//		case IL_OUT_OF_MEMORY:
	//			ERR( "Unable to convert texture image " << _filename << ". Could not allocate memory for the converted image data." << std::endl );
	//			break;
	//		default:
	//			ERR( "Unable to convert texture image " << _filename << ". Unknown error code" << std::endl );
	//			break;
	//		}
	//	}
	//}
	//else
	//{
	//	switch ( ilGetError( ) )
	//	{
	//	case IL_COULD_NOT_OPEN_FILE:
	//		ERR( "Unable to load texture image " << _filename << ". File does not exist or is locked" << std::endl );
	//		break;
	//	case IL_ILLEGAL_OPERATION:
	//		ERR( "Unable to load texture image " << _filename << ". There is currently no image bound." << std::endl );
	//		break;
	//	case IL_INVALID_EXTENSION:
	//		ERR( "Unable to load texture image " << _filename << ". The file could not be loaded based on extension or header." << std::endl );
	//		break;
	//	case IL_INVALID_PARAM:
	//		ERR( "Unable to load texture image " << _filename << ". File name was not valid." << std::endl );
	//		break;
	//	default:
	//		ERR( "Unable to load texture image " << _filename << ". Unknown error code" << std::endl );
	//		break;
	//	}
	//}
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