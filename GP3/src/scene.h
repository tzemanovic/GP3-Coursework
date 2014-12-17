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
		virtual void vRenderChildren( const Scene& scene, const Time& time, std::shared_ptr< Camera > camera ) override;
	};

public:
	Scene( );
	virtual ~Scene( );
public:
	void render( const Time& time, std::shared_ptr< Camera > camera );
	void update( const Time& time );
	void addSceneNode( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass );
public:
	void setAmbientLightColor( const glm::vec3& color )
	{
		_ambientLightColor = color;
	}
	void setDiffuseLightColor( const glm::vec3& color )
	{
		_diffuseLightColor = color;
	}
	void setLightDirection( const glm::vec3& dir )
	{
		_lightDirection = dir;
	}
	const glm::vec3& getAmbientLightColor( ) const
	{
		return _ambientLightColor;
	}
	const glm::vec3& getDiffuseLightColor( ) const
	{
		return _diffuseLightColor;
	}
	const glm::vec3& getLightDirection( ) const
	{
		return _lightDirection;
	}
protected:
	RootSceneNode 	_rootNode;
	glm::vec3 		_ambientLightColor;
	glm::vec3 		_diffuseLightColor;
	glm::vec3 		_lightDirection;
};