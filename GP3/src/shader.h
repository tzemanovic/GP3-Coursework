#pragma once

class Shader;

class ShaderProgram
{
public:
	ShaderProgram( );
	~ShaderProgram( );
public:
	void addShader( Shader* shader );
	void use( );
	void setProjection( const glm::mat4& projection );
	void setView( const glm::mat4& view );
	void setWorld( const glm::mat4& world );
	void setTextureUnit( const unsigned textureUnit );
private:
	GLuint _program;
};

class Shader
{
	friend class ShaderProgram;
public:
	Shader( String&& filename, GLenum type );
	~Shader( );
private:
	GLuint _shader;
};
