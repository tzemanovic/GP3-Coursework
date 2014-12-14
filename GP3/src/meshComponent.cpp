#include "pch.h"
#include "meshComponent.h"
#include "scene.h"
#include "gameObject.h"

MeshComponent::MeshComponent( Scene& scene, String filename ) : RenderComponent( ComponentType::Mesh )
{
	std::shared_ptr< SceneNode > sceneNode( new SceneNode( this ) );
	scene.addSceneNode( sceneNode, RenderPassType::GameObject );
}