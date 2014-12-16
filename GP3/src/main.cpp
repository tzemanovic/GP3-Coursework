#include "pch.h"
#include "game.h"

int main( )
{
	WindowConfig windowConfig;
	OpenGlConfig openGlConfig;
	Game game( "GP3 coursework", windowConfig, openGlConfig );

	std::shared_ptr< PlayerView > view( new PlayerView( ) );
	game.addView( view );

	std::shared_ptr< GameObject > g1( new GameObject( ) );
	g1->translate( glm::vec3( 0.0f, 0.0f, -5.0f ) );
	g1->setOnUpdate( [&]( GameObject& gameObject, const Time& time )
	{
		gameObject.setY( std::sinf( time.currentMs * 0.005f ) );
		gameObject.rotate( glm::vec3( 0.0f, 1.0f, 0.0f ), time.deltaMs * 0.001f );
	} );
	std::shared_ptr<MeshComponent> g1mesh( new MeshComponent( "assets/models/crate/Crate1.3ds" ) );
	GameObject::addComponent( g1, g1mesh );
	game.addGameObject( g1 );

	game.run( );

	return 0;
}