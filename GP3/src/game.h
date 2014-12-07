#pragma once

#include "window.h"

class Game
{
public:
	Game( string windowName, const WindowConfig& windowConfig, OpenGlConfig& openGlConfig = OpenGlConfig( ) );
	~Game( );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
public:
	void run( );
private:
	void render( const double deltaMs );
	void update( const double deltaMs );
private:
	Window* _window;
};