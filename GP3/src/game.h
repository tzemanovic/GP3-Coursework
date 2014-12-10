#pragma once

#include "window.h"

class View;
class GameObject;
struct Time;

class Game
{
public:
	Game( String windowName, const WindowConfig& windowConfig, OpenGlConfig& openGlConfig = OpenGlConfig( ) );
	~Game( );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
public:
	void run( );
	void addView( View* );
	void addGameObject( std::shared_ptr< GameObject > gameObject );
private:
	void render( const Time& time );
	void update( const Time& time );
private:
	GameObjectId											_lastId;
	Window*													_window;
	std::map< GameObjectId, std::shared_ptr< GameObject > >	_gameObjects;
	std::vector< View* >									_views;
};