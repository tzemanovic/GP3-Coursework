#include "pch.h"
#include "scene.h"
#include "openGl.h"

Scene::Scene( ) : _rootNode( )
{

}
Scene::~Scene( )
{

}
void Scene::render( const Time& time, std::shared_ptr< Camera > camera )
{
	_rootNode.vRender( *this, time, camera );
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
	_children.resize( +RenderPassType::Count );

	std::shared_ptr< SceneNode > firstGroup( new SceneNode( ) );
	_children[+RenderPassType::First] = firstGroup;
	std::shared_ptr< SceneNode > skyGroup( new SceneNode( ) );
	_children[+RenderPassType::Sky] = skyGroup;
	std::shared_ptr< SceneNode > gameObjectsGroup( new SceneNode( ) );
	_children[+RenderPassType::GameObject] = gameObjectsGroup;
	std::shared_ptr< SceneNode > hudGroup( new SceneNode( ) );
	_children[+RenderPassType::HUD] = hudGroup;
}
void Scene::RootSceneNode::addChild( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass )
{
	_children[+renderPass]->addChild( sceneNode );
}
void Scene::RootSceneNode::vRenderChildren( const Scene& scene, const Time& time, std::shared_ptr< Camera > camera )
{
	// render children in order of their render pass
	for ( RenderPassType i = RenderPassType::First; i < RenderPassType::Count; ++i )
	{
		switch ( i )
		{
		case RenderPassType::Sky:
		case RenderPassType::GameObject:
		case RenderPassType::HUD:
			_children[+i]->vRenderChildren( scene, time, camera );
			break;
		}
	}
}