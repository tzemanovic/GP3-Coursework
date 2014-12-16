#pragma once

#include "renderComponent.h"
#include "vertex.h"

class Scene;
class Texture;
class ShaderProgram;

class MeshComponent : public RenderComponent
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
	MeshComponent( String&& filename );
	virtual ~MeshComponent( );
public:
	virtual void vRender( const Time& time, std::shared_ptr< Camera > camera, const glm::mat4& toWorld ) override;
	virtual void vInit( Game& game ) override;
private:
	void init( const aiScene* scene );
	void initMesh( Mesh& mesh, const aiMesh* importMesh );
	void initTexture( const aiMaterial* importTexture );
protected:
	std::vector<Mesh>		_meshes;
	std::vector<Texture*>	_textures;
	String 					_filename;
	ShaderProgram* 			_shaders;
};