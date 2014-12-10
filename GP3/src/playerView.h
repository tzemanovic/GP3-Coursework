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
	virtual void vRender( const Time& time, OpenGl* openGl ) override;
	virtual void vUpdate( const Time& time ) override;
	virtual const bool vProcessMessage( const InputMessage& message ) override;
protected:
	std::shared_ptr< Camera >	_camera;
	Scene						_scene;
};