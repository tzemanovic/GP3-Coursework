#include "pch.h"
#include "meshModel.h"
#include "game.h"
#include "openGl.h"
#include "texture.h"
#include "stringUtils.h"
#include "camera.h"

MeshModel::Mesh::Mesh( )
{

}
void MeshModel::Mesh::init( const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices )
{
	numIndices = indices.size( );
	// VAO
	glGenVertexArrays( 1, &vertexArray );
	glBindVertexArray( vertexArray );
	glErrorCheck( );
	// VBO
	glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * vertices.size( ), &vertices[0], GL_STATIC_DRAW );
	glErrorCheck( );
	// IBO
	glGenBuffers( 1, &indexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * numIndices, &indices[0], GL_STATIC_DRAW );
	glErrorCheck( );
	// enable vertex attributes
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );
	glEnableVertexAttribArray( 2 );
	glErrorCheck( );
	// define vertex attributes
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), 0 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( const GLvoid* ) 12 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( const GLvoid* ) 24 );
	glErrorCheck( );
}
MeshModel::Mesh::~Mesh( )
{
	if ( vertexArray != 0 )
	{
		glDeleteBuffers( 1, &vertexArray );
	}
	if ( vertexBuffer != 0 )
	{
		glDeleteBuffers( 1, &vertexBuffer );
	}
	if ( indexBuffer != 0 )
	{
		glDeleteBuffers( 1, &indexBuffer );
	}
}


MeshModel::MeshModel( String&& filename, const Game& game, const GLenum texturesTarget ) : _filename( filename ), _shaders( nullptr ), _texturesTarget( texturesTarget )
{
	Assimp::Importer importer;
	importer.SetPropertyFloat( AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f );
	const aiScene* scene = importer.ReadFile( _filename.c_str( ), aiProcess_Triangulate | aiProcess_GenSmoothNormals );
	if ( scene != nullptr )
	{
		load( scene );
		_shaders = game.getDefaultShaders( );
	}
	else
	{
		ERR( "Error parsing " << _filename << ". Error message: " << importer.GetErrorString( ) );
	}
}
MeshModel::~MeshModel( )
{
	for each ( auto tex in _textures )
	{
		delete tex;
		tex = nullptr;
	}
	_textures.clear( );
}
void MeshModel::vRender( const Scene& scene, const Time& time, std::shared_ptr< Camera > camera, const glm::mat4& toWorld )
{
	_shaders->use( );
	// set camera view and projection matrix
	_shaders->setProjection( camera->getProjection( ) );
	_shaders->setView( camera->getView( ) );
	_shaders->setLightDirection( scene.getLightDirection( ) );
	_shaders->setAmbientLightColor( scene.getAmbientLightColor( ) );
	_shaders->setDiffuseLightColor( scene.getDiffuseLightColor( ) );
	for ( unsigned int i = 0; i < _meshes.size( ); i++ )
	{
		// bind vertex array object
		glBindVertexArray( _meshes[i].vertexArray );
		glErrorCheck( );
		// bind texture
		const unsigned materialIndex = _meshes[i].materialIndex;
		const unsigned textureUnit = 0;
		if ( _textures[materialIndex] )
		{
			_shaders->setTextureUnit( textureUnit );
			_textures[materialIndex]->bind( textureUnit );
		}
		// set object's world matrix
		_shaders->setWorld( toWorld * glm::mat4_cast( _meshRot ) );
		// draw mesh
		glDrawArrays( GL_TRIANGLES, 0, _meshes[i].numIndices );
	}
	glErrorCheck( );
}
void MeshModel::load( const aiScene* scene )
{
	// initalise meshes
	unsigned meshCount = scene->mNumMeshes;
	_meshes.resize( meshCount );
	for ( unsigned i = 0; i < meshCount; ++i )
	{
		loadMesh( _meshes[i], scene->mMeshes[i] );
	}
	// initialise textures
	for ( unsigned i = 0; i < scene->mNumMaterials; ++i )
	{
		loadTexture( scene->mMaterials[i] );
	}
}
void MeshModel::loadMesh( Mesh& mesh, const aiMesh* importMesh )
{
	mesh.materialIndex = importMesh->mMaterialIndex;
	std::vector< Vertex > vertices;
	std::vector< unsigned > indices;
	const aiVector3D origin;
	for ( unsigned i = 0; i < importMesh->mNumVertices; ++i )
	{
		const aiVector3D* pPos = &( importMesh->mVertices[i] );
		const aiVector3D* pNormal = &( importMesh->mNormals[i] );
		const aiVector3D* pTexCoord = importMesh->HasTextureCoords( 0 ) ? &( importMesh->mTextureCoords[0][i] ) : &origin;
		Vertex v{
			glm::vec3( pPos->x, pPos->y, pPos->z ),
			glm::vec3( pNormal->x, pNormal->y, pNormal->z ),
			glm::vec2( pTexCoord->x, pTexCoord->y )
		};
		vertices.push_back( v );
	}
	for ( unsigned int i = 0; i < importMesh->mNumFaces; ++i )
	{
		const aiFace& Face = importMesh->mFaces[i];
		indices.push_back( Face.mIndices[0] );
		indices.push_back( Face.mIndices[1] );
		indices.push_back( Face.mIndices[2] );
	}
	mesh.init( vertices, indices );
}
void MeshModel::loadTexture( const aiMaterial* importTexture )
{
	aiString path;
	aiReturn texFound = importTexture->GetTexture( aiTextureType_DIFFUSE, 0, &path );
	Texture* texture = new Texture( _texturesTarget, basePath( _filename ) + path.data );
	_textures.push_back( texture );
}