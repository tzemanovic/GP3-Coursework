#pragma once

#include "view.h"
#include "scene.h"

class PlayerView : public View
{
public:
	PlayerView( );
	virtual ~PlayerView( );
	PlayerView( const PlayerView& ) = delete;
	PlayerView& operator=( const PlayerView& ) = delete;
public:
	virtual void vInit( Game& game ) override;
	virtual void vAddSceneNode( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass ) override;
	virtual void vRender( const Time& time ) override;
	virtual void vUpdate( const Time& time ) override;
	virtual const bool vProcessMessage( const InputMessage& message ) override;
public:
	Scene& getScene();
protected:
	std::shared_ptr< Camera >	_camera;
	Scene						_scene;
};