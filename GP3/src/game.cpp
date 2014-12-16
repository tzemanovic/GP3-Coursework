#include "pch.h"
#include "game.h"
#include "window.h"
#include "view.h"
#include "gameObject.h"
#include "time.h"
#include "openGl.h"

Game::Game( String&& windowName, const WindowConfig& windowConfig, OpenGlConfig& openGlConfig ) : _lastId( 0 ), _window( nullptr ), _vertexShader( nullptr ), _fragmentShader( nullptr ),
_defaultShaders( nullptr )
{
	// open a window
	_window = new Window( std::move( windowName ), windowConfig, openGlConfig );
	_vertexShader = new Shader( "assets/shaders/shader.vert", GL_VERTEX_SHADER );
	_fragmentShader = new Shader( "assets/shaders/shader.frag", GL_FRAGMENT_SHADER );
	_defaultShaders = new ShaderProgram( );
	_defaultShaders->addShader( _vertexShader );
	_defaultShaders->addShader( _fragmentShader );
}
Game::~Game( )
{
	delete _vertexShader;
	_vertexShader = nullptr;
	delete _fragmentShader;
	_fragmentShader = nullptr;
	delete _defaultShaders;
	_defaultShaders = nullptr;
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
		Time time{ static_cast< float >( currentMs ), static_cast< float >( deltaMs ) };
		/*double fps = 1000 / deltaMs;
		LOG( fps  << std::endl );*/
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
				else if ( msg.key.type == Key::F1 )
				{
					OpenGl::fillMode( );
				}
				else if ( msg.key.type == Key::F2 )
				{
					OpenGl::wireframeMode( );
				}
				else if ( msg.key.type == Key::F3 )
				{
					OpenGl::pointMode( );
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
void Game::addView( std::shared_ptr< View > view )
{
	view->vInit( *this );
	_views.push_back( view );
}
void Game::addGameObject( std::shared_ptr< GameObject > gameObject )
{
	GameObjectId id = ++_lastId;
	gameObject->setId( id );
	gameObject->init( *this );
	_gameObjects.insert( std::make_pair( id, gameObject ) );
}
const float Game::windowAspectRatio( ) const
{
	return static_cast< float >( _window->getWidth( ) ) / static_cast< float >( _window->getHeight( ) );
}
void Game::render( const Time& time )
{
	_window->clear( 0.f, 0.f, 0.f );
	for each ( auto view in _views )
	{
		view->vRender( time );
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
void Game::addSceneNode( std::shared_ptr< SceneNode > sceneNode, RenderPassType renderPass )
{
	for each( auto view in _views )
	{
		view->vAddSceneNode( sceneNode, renderPass );
	}
}