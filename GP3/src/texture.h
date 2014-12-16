#pragma once

class Texture
{
public:
	Texture( GLenum textureTarget, const String& filename );
	~Texture( );
public:
	void bind( GLint textureUnit );
private:
	GLuint _texture;
	GLuint _sampler;
	GLenum _textureTarget;
	String _filename;
};