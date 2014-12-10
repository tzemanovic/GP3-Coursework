#include "pch.h"
#include "game.h"
#include "window.h"
#include "view.h"
#include "gameObject.h"
#include "time.h"

Game::Game( String windowName, const WindowConfig& windowConfig, OpenGlConfig& openGlConfig ) : _lastId( 0 ), _window( nullptr )
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
		Time time{ currentMs, deltaMs };
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
		if ( running )
		{
			render( time );
			update( time );
		}
	}
}
void Game::addView( View* view )
{
	_views.push_back( view );
}
void Game::addGameObject( std::shared_ptr< GameObject > gameObject )
{
	GameObjectId id = ++_lastId;
	gameObject->setId( id );
	_gameObjects.insert( std::make_pair( id, gameObject ) );
}
void Game::render( const Time& time )
{
	_window->clear( 0.f, 0.f, 0.f );
	for each ( auto view in _views )
	{
		view->vRender( time, _window->getOpenGl( ) );
	}
	_window->display( );
}
void Game::update( const Time& time )
{
	// update views
	for each ( auto view in _views )
	{
		view->vUpdate( time );
	}
	// update game objects
	for each ( auto gameObject in _gameObjects )
	{
		gameObject.second->update( time );
	}
}