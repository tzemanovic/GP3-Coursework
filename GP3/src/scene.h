#pragma once

#include "sceneNode.h"
#include "renderPassType.h"

struct Time;

class Scene
{
private:

	class RootSceneNode : public SceneNode
	{
	public:
		RootSceneNode( );
	public:
		void addChild( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass );
	public:
		virtual void vRenderChildren( Scene *scene, const Time& time, std::shared_ptr< Camera > camera ) override;
	};

public:
	Scene( );
	virtual ~Scene( );
public:
	void render( const Time& time, std::shared_ptr< Camera > camera );
	void update( const Time& time );
	void addSceneNode( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass );
protected:
	RootSceneNode _rootNode;
};