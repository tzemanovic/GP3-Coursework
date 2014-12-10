#include "pch.h"
#include "playerView.h"
#include "time.h"

PlayerView::PlayerView( ) : View( ), _camera( nullptr ), _scene( )
{

}
PlayerView::~PlayerView( )
{

}
void PlayerView::vRender( const Time& time, OpenGl* openGl )
{
	_scene.render( time, openGl, _camera );
}
void PlayerView::vUpdate( const Time& time )
{

}
const bool PlayerView::vProcessMessage( const InputMessage& message )
{
	return false;
}