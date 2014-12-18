#include "pch.h"
#include "playerView.h"
#include "time.h"
#include "camera.h"
#include "controller.h"
#include "game.h"

PlayerView::PlayerView( ) : View( ), _camera( nullptr ), _controller( nullptr ), _scene( ), _isFreeCameraController( false )
{
	_camera.reset( new Camera( ) );
	_scene.addSceneNode( _camera, RenderPassType::First );
}
PlayerView::~PlayerView( )
{

}
void PlayerView::setCameraOffset( const glm::vec3& offset )
{
	_camera->setOffset( offset );
}
void PlayerView::setCameraRotation( const glm::quat& rot )
{
	_camera->setRotation( rot );
}
void PlayerView::vInit( Game& game )
{
	_camera->setAspectRatio( game.windowAspectRatio( ) );
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
	if ( _controller != nullptr )
	{
		_controller->update( time );
	}
	_scene.update( time );
}
const bool PlayerView::vProcessMessage( const InputMessage& message )
{
	// toggle between cameras when F key is pressed
	if ( message == InputMessage::Type::KeyUp )
	{
		if ( message.key.type == Key::F )
		{
			if ( _isFreeCameraController )
			{
				_camera->setTarget( _owner );
				_isFreeCameraController = false;
			}
			else
			{
				_camera->setTarget( nullptr );
				_isFreeCameraController = true;
			}
			return true;
		}
	}
	if ( _controller != nullptr )
	{
		return _controller->processMessage( message );
	}
	return false;
}
void PlayerView::vOwn( std::shared_ptr< GameObject > owner )
{
	_owner = owner;
	_camera->setTarget( _owner );
}