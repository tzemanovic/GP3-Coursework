#include "pch.h"
#include "skyboxMeshModel.h"
#include "shader.h"

SkyboxMeshModel::SkyboxMeshModel( const Game& game ) : MeshModel( "assets/models/skybox/skybox1.obj", game )
{
	Shader vert( "assets/shaders/skybox.vert", GL_VERTEX_SHADER );
	Shader frag( "assets/shaders/skybox.frag", GL_FRAGMENT_SHADER );
	_shaders = new ShaderProgram( );
	_shaders->addShader( &vert );
	_shaders->addShader( &frag );
}
SkyboxMeshModel::~SkyboxMeshModel( )
{
	delete _shaders;
	_shaders = nullptr;
}
void SkyboxMeshModel::vRender( const Scene& scene, const Time& time, std::shared_ptr< Camera > camera, const glm::mat4& toWorld )
{
	// temporarily disable depth buffer and backface culling for skybox, so that nothing goes behind it
	glDepthMask( GL_FALSE );
	glDisable( GL_CULL_FACE );
	MeshModel::vRender( scene, time, camera, toWorld );
	glEnable( GL_CULL_FACE );
	glDepthMask( GL_TRUE );
}