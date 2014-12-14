#include "pch.h"
#include "meshComponent.h"
#include "scene.h"
#include "gameObject.h"

MeshComponent::MeshComponent( Scene& scene, String filename ) : RenderComponent( ComponentType::Mesh )
{
	std::shared_ptr< SceneNode > sceneNode( new SceneNode( this ) );
	scene.addSceneNode( sceneNode, RenderPassType::GameObject );

	Assimp::Importer importer;
	std::wstring filenameWS( filename );
	const aiScene* import = importer.ReadFile( std::string( filenameWS.begin( ), filenameWS.end( ) ), aiProcess_Triangulate | aiProcess_GenSmoothNormals );
	if ( import != nullptr )
	{
		//InitFromScene( pScene, Filename );
	}
	else
	{
		LOG( "Error importing file: \"" << filename << "\". Error message: " << importer.GetErrorString( ) );
	}
}