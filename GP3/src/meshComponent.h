#pragma once

#include "renderComponent.h"
#include "vertex.h"

class Scene;
class Texture;

class MeshComponent : public RenderComponent
{
public:
	MeshComponent( Scene& scene, String filename );
protected:

	struct Mesh
	{
	public:
		//Mesh( const std::vector<Vertex>& vertices,	const std::vector<unsigned int>& indices );
		//~Mesh( );
	public:
		GLuint		vertexBuffer;
		GLuint		indexBuffer;
		unsigned	numIndices;
		unsigned	materialIndex;
	};

	std::vector<Mesh>		_meshes;
	std::vector<Texture*>	_textures;
};