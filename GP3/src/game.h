#pragma once

#include "window.h"
#include "gameObject.h"
#include "playerView.h"
#include "meshComponent.h"
#include "meshModel.h"
#include "skyboxMeshModel.h"
#include "audioManager.h"
#include "audioComponent.h"
#include "time.h"
#include "shader.h"
#include "controller.h"

class Game
{
public:
	Game( String&& windowName, const WindowConfig& windowConfig, OpenGlConfig& openGlConfig = OpenGlConfig( ) );
	~Game( );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
public:
	void run( );
	void addView( std::shared_ptr< View > );
	void addGameObject( std::shared_ptr< GameObject > gameObject );
	const float windowAspectRatio( ) const;
	void addSceneNode( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass );
public:
	ShaderProgram* getDefaultShaders( ) const
	{
		return _defaultShaders;
	}
private:
	void render( const Time& time );
	void update( const Time& time );
	void passMessage( const InputMessage& msg );
private:
	GameObjectId											_lastId;
	Window*													_window;
	std::map< GameObjectId, std::shared_ptr< GameObject > >	_gameObjects;
	std::vector< std::shared_ptr< View > >					_views;
	ShaderProgram* 											_defaultShaders;
};