#include "pch.h"
#include "scene.h"
#include "openGl.h"

Scene::Scene( ) : _rootNode( )
{

}
Scene::~Scene( )
{

}
void Scene::render( const Time& time, OpenGl* openGl, std::shared_ptr< Camera > camera )
{
	_rootNode.vRender( this, time, openGl, camera );
}
void Scene::update( const Time& time )
{

}
void Scene::addSceneNode( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass )
{
	_rootNode.addChild( sceneNode, renderPass );
}
Scene::RootSceneNode::RootSceneNode( ) : SceneNode( )
{
	_children.reserve( RenderPassType::Count );

	std::shared_ptr< SceneNode > firstGroup( new SceneNode( ) );
	addChild( firstGroup, RenderPassType::First );
	std::shared_ptr< SceneNode > skyGroup( new SceneNode( ) );
	addChild( skyGroup, RenderPassType::Sky );
	std::shared_ptr< SceneNode > gameObjectsGroup( new SceneNode( ) );
	addChild( gameObjectsGroup, RenderPassType::GameObject );
	std::shared_ptr< SceneNode > hudGroup( new SceneNode( ) );
	addChild( hudGroup, RenderPassType::HUD );
}
void Scene::RootSceneNode::addChild( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass )
{
	_children[renderPass]->addChild( sceneNode );
}
void Scene::RootSceneNode::vRenderChildren( Scene* scene, const Time& time, OpenGl* openGl, std::shared_ptr< Camera > camera )
{
	// render children in order of their render pass
	for ( unsigned i = RenderPassType::First; i < RenderPassType::Count; ++i )
	{
		switch ( i )
		{
		case RenderPassType::Sky:
		case RenderPassType::GameObject:
		case RenderPassType::HUD:
			_children[i]->vRenderChildren( scene, time, openGl, camera );
			break;
		}
	}
}