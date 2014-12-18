#pragma once

#include "vertex.h"

class Scene;
class Texture;
class Game;
class Camera;
class ShaderProgram;
struct Time;

class MeshModel
{
protected:
	struct Mesh
	{
	public:
		Mesh( );
		void init( const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices );
		~Mesh( );
	public:
		GLuint 		vertexArray;
		GLuint		vertexBuffer;
		GLuint		indexBuffer;
		unsigned	numIndices;
		unsigned	materialIndex;
	};

public:
	MeshModel( String&& filename, const Game& game, const GLenum texturesTarget = GL_TEXTURE_2D );
	virtual ~MeshModel( );
public:
	virtual void vRender( const Scene& scene, const Time& time, std::shared_ptr< Camera > camera, const glm::mat4& toWorld );
public:
	void setMeshRot( const glm::quat& rot )
	{
		_meshRot = rot;
	}
	void rotateMesh( const glm::vec3& axis, const float angle )
	{
		_meshRot = glm::rotate( _meshRot, angle, axis );
	}
	const float getRadius( const glm::vec3& scale ) const
	{
		return std::cbrt(
			std::pow( ( _maxExtents.x - _minExtents.x ) * scale.x, 2 ) +
			std::pow( ( _maxExtents.y - _minExtents.y ) * scale.y, 2 ) +
			std::pow( ( _maxExtents.z - _minExtents.z ) * scale.z, 2 ) );
	}
protected:
	void load( const aiScene* scene );
	void loadMesh( Mesh& mesh, const aiMesh* importMesh );
	void calcExtents( const aiVector3D* pos );
	void loadTexture( const aiMaterial* importTexture );
protected:
	glm::quat 				_meshRot;
	glm::vec3 				_minExtents;
	glm::vec3 				_maxExtents;
	ShaderProgram* 			_shaders;
	std::vector<Mesh>		_meshes;
	std::vector<Texture*>	_textures;
	GLenum 					_texturesTarget;
	String 					_filename;
};