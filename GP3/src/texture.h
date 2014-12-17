#pragma once

class Texture
{
public:
	Texture( const GLenum textureTarget, const String& filename );
	~Texture( );
public:
	void bind( const GLint textureUnit );
private:
	GLuint _texture;
	GLuint _sampler;
	GLenum _textureTarget;
	String _filename;
};