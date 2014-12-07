#include "pch.h"
#include "playerView.h"

PlayerView::PlayerView( ) : View( ), _camera( nullptr )
{

}
PlayerView::~PlayerView( )
{

}
void PlayerView::vRender( const float deltaMs )
{

};
void PlayerView::vUpdate( const float deltaMs )
{

}
const bool PlayerView::vProcessMessage( const InputMessage& message )
{
	return false;
}