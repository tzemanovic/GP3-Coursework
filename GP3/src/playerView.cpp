#include "pch.h"
#include "playerView.h"
#include "time.h"
#include "camera.h"
#include "game.h"

PlayerView::PlayerView( ) : View( ), _camera( nullptr ), _scene( )
{

}
PlayerView::~PlayerView( )
{

}
void PlayerView::vInit( Game& game )
{
	_camera.reset( new Camera( game.windowAspectRatio( ) ) );
}
void PlayerView::vAddSceneNode( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass )
{
	_scene.addSceneNode( sceneNode, renderPass );
}
void PlayerView::vRender( const Time& time )
{
	_scene.render( time, _camera );
}
void PlayerView::vUpdate( const Time& time )
{

}
const bool PlayerView::vProcessMessage( const InputMessage& message )
{
	return false;
}
Scene& PlayerView::getScene( )
{
	return _scene;
}