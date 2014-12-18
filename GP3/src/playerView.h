#pragma once

#include "view.h"
#include "scene.h"

class Controller;

class PlayerView : public View
{
public:
	PlayerView( );
	virtual ~PlayerView( );
	PlayerView( const PlayerView& ) = delete;
	PlayerView& operator=( const PlayerView& ) = delete;
public:
	void setCameraOffset( const glm::vec3& offset );
	void setCameraRotation( const glm::quat& rot );
public:
	void setAmbientLightColor( const glm::vec3& color )
	{
		_scene.setAmbientLightColor( color );
	}
	void setDiffuseLightColor( const glm::vec3& color )
	{
		_scene.setDiffuseLightColor( color );
	}
	void setLightDirection( const glm::vec3& dir )
	{
		_scene.setLightDirection( dir );
	}
	void setController( std::shared_ptr< Controller > controller )
	{
		_controller = controller;
	}
public:
	virtual void vInit( Game& game ) override;
	virtual void vAddSceneNode( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass ) override;
	virtual void vRender( const Time& time ) override;
	virtual void vUpdate( const Time& time ) override;
	virtual const bool vProcessMessage( const InputMessage& message ) override;
	virtual void vOwn( std::shared_ptr< GameObject > owner ) override;
protected:
	std::shared_ptr< Camera >		_camera;
	std::shared_ptr< GameObject >	_owner;
	std::shared_ptr< Controller >	_controller;
	Scene							_scene;
	bool 							_isFreeCameraController;
};