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
private:
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
	MeshModel( String&& filename, const Game& game );
	~MeshModel( );
public:
	void render( const Scene& scene, const Time& time, std::shared_ptr< Camera > camera, const glm::mat4& toWorld );
	//void init( Game& game );
public:
	void setMeshRot( const glm::quat& rot )
	{
		_meshRot = rot;
	}
	void rotateMesh( const glm::vec3& axis, const float angle )
	{
		_meshRot = glm::rotate( _meshRot, angle, axis );
	}
private:
	void load( const aiScene* scene );
	void loadMesh( Mesh& mesh, const aiMesh* importMesh );
	void loadTexture( const aiMaterial* importTexture );
private:
	glm::quat 				_meshRot;
	ShaderProgram* 			_shaders;
	std::vector<Mesh>		_meshes;
	std::vector<Texture*>	_textures;
	String 					_filename;
};