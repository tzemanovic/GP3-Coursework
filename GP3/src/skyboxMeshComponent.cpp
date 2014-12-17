#include "pch.h"
#include "skyboxMeshComponent.h"
#include "game.h"

SkyboxMeshComponent::SkyboxMeshComponent( MeshModel& model ) : MeshComponent( model )
{

}
SkyboxMeshComponent::~SkyboxMeshComponent( )
{

}
void SkyboxMeshComponent::vInit( Game& game )
{
	std::shared_ptr< SceneNode > sceneNode( new SceneNode( this ) );
	game.addSceneNode( sceneNode, RenderPassType::Sky );
}