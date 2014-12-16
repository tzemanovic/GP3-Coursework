#pragma once

class Scene;
class OpenGl;
class Camera;
class RenderComponent;
struct Time;

class SceneNode
{
public:
	SceneNode( RenderComponent* renderComponent = nullptr );
	virtual ~SceneNode( );
	void* operator new( size_t i )
	{
		// change alignment to 16-bit
		return _mm_malloc( i, 16 );
	}
		void operator delete( void* p )
	{
		_mm_free( p );
	}
public:
	void setParent( SceneNode* sceneNode );
	void addChild( std::shared_ptr< SceneNode > sceneNode );
	const glm::vec3 getPosition( ) const;
	const glm::vec3 getWorldPosition( ) const;
public:
	virtual bool vIsVisible( Scene* scene ) const;
	virtual void vPreRender( Scene* scene );
	virtual void vRender( Scene* scene, const Time& time, std::shared_ptr< Camera > camera );
	virtual void vPostRender( Scene* scene );
	virtual void vRenderChildren( Scene* scene, const Time& time, std::shared_ptr< Camera > camera );
	virtual void vUpdate( Scene* scene, const Time& time );
protected:
	GameObjectId								_gameObjectId;
	SceneNode*									_parent;
	RenderComponent*							_renderComponent;
	glm::mat4									_toWorld, _toWorldNonScaled, _fromWorld;
	std::vector< std::shared_ptr< SceneNode > > _children;
};