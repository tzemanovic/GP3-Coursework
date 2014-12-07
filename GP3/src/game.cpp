#include "pch.h"
#include "game.h"
#include "window.h"

Game::Game( string windowName, const WindowConfig& windowConfig, OpenGlConfig& openGlConfig ) : _window( nullptr )
{
	_window = new Window( windowName, windowConfig, openGlConfig );
}
Game::~Game( )
{
	if ( _window != nullptr )
	{
		_window->close( );
		delete( _window );
		_window = nullptr;
	}
}
void Game::run( )
{
	bool running = true;
	double startTime = Window::time( );
	double previousTime = 0.0;
	while ( running )
	{
		double currentMs = Window::time( ) - startTime;
		double deltaMs = currentMs - previousTime;
		double fps = 1000 / deltaMs;
		std::cout << ( fps ) << '\n';
		previousTime = currentMs;
		InputMessage msg;
		while ( _window->popMessage( msg ) )
		{
			switch ( msg.type )
			{
			case InputMessage::Type::Close:
			{
				_window->close( );
				running = false;
				break;
			}
			case InputMessage::Type::KeyDown:
			{
				if ( msg.key.type == Key::F4 && msg.key.lAlt )
				{
					_window->close( );
					running = false;
					break;
				}
			}
			case InputMessage::Type::KeyUp:
			case InputMessage::Type::MouseButtonDown:
			case InputMessage::Type::MouseButtonUp:
			case InputMessage::Type::MouseMove:
			case InputMessage::Type::MouseWheel:
			{
				break;
			}
			}
		}
		render( deltaMs );
		update( deltaMs );
	}
}
void Game::render( const double deltaMs )
{

}
void Game::update( const double deltaMs )
{

}