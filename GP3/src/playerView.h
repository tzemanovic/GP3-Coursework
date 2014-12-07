#pragma once

#include "view.h"

class PlayerView : public View
{
public:
	PlayerView( );
	virtual ~PlayerView( );
	PlayerView( const PlayerView& ) = delete;
	PlayerView& operator=( const PlayerView& ) = delete;
public:
	virtual void vRender( const float deltaMs ) override;
	virtual void vUpdate( const float deltaMs ) override;
	virtual const bool vProcessMessage( const InputMessage& message ) override;
protected:
	std::shared_ptr< Camera >		_camera;
};