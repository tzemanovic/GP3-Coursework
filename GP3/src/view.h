#pragma once

#include "renderPassType.h"

class InputMessage;
class SceneNode;
class Game;
class GameObject;
struct Time;

class View
{
public:
	View( );
	virtual ~View( );
	View( const View& ) = delete;
	View& operator=( const View& ) = delete;
public:
	virtual void vInit( Game& game ) = 0;
	virtual void vAddSceneNode( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass ) = 0;
	virtual void vRender( const Time& time ) = 0;
	virtual void vUpdate( const Time& time ) = 0;
	virtual const bool vProcessMessage( const InputMessage& message ) = 0;
	virtual void vOwn( std::shared_ptr< GameObject > owner ) = 0;
};