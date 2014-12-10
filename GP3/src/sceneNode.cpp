#include "pch.h"
#include "sceneNode.h"
#include "openGl.h"
#include "camera.h"
#include "renderComponent.h"
#include "gameObject.h"

SceneNode::SceneNode( RenderComponent* renderComponent ) : _parent( nullptr ), _renderComponent( renderComponent )
{

}
SceneNode::~SceneNode( )
{
	
}
void SceneNode::setParent( SceneNode* sceneNode )
{
	_parent = sceneNode;
}
void SceneNode::addChild( std::shared_ptr< SceneNode > sceneNode )
{
	_children.push_back( sceneNode );
	sceneNode->setParent( this );
}
bool SceneNode::vIsVisible( Scene* scene ) const
{
	return true;
}
void SceneNode::vPreRender( Scene* scene )
{
	if ( _renderComponent != nullptr )
	{
		auto gameObject = _renderComponent->getOwner( );
		_toWorld = gameObject->getTransform( );
		_toWorldNonScaled = gameObject->getTransformNonScaled( );
		_fromWorld =  glm::inverse( _toWorld );
	}
}
void SceneNode::vRender( Scene* scene, const Time& time, OpenGl* openGl, std::shared_ptr< Camera > camera )
{
	if ( _renderComponent != nullptr )
	{
		_renderComponent->vRender( time, openGl, camera );
	}
	vRenderChildren( scene, time, openGl, camera );
}
void SceneNode::vPostRender( Scene* scene )
{

}
void SceneNode::vRenderChildren( Scene* scene, const Time& time, OpenGl* openGl, std::shared_ptr< Camera > camera )
{
	for each ( auto child in _children )
	{
		if ( child->vIsVisible( scene ) )
		{
			child->vPreRender( scene );
			child->vRender( scene, time, openGl, camera );
			child->vRenderChildren( scene, time, openGl, camera );
			child->vPostRender( scene );
		}
	}
}
void SceneNode::vUpdate( Scene* scene, const Time& time )
{
	for ( auto i = _children.begin( ), end = _children.end( ); i != end; ++i )
	{
		( *i )->vUpdate( scene, time );
	}
}