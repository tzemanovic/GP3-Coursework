#pragma once

#include "sceneNode.h"
#include "renderPassType.h"

class OpenGl;
struct Time;

class Scene
{
public:
	Scene( );
	virtual ~Scene( );
public:
	void render( const Time& time, OpenGl* openGl, std::shared_ptr< Camera > camera );
	void update( const Time& time );
	void addSceneNode( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass );
private:

	class RootSceneNode : public SceneNode
	{
	public:
		RootSceneNode( );
	public:
		void addChild( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass );
	public:
		virtual void vRenderChildren( Scene *scene, const Time& time, OpenGl* openGl, std::shared_ptr< Camera > camera ) override;
	};

protected:
	RootSceneNode _rootNode;
};