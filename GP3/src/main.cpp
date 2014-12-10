#include "pch.h"
#include "game.h"
#include "gameObject.h"

int main( )
{
	WindowConfig windowConfig;
	Game game( L"GP3 coursework", windowConfig );
	std::shared_ptr< GameObject > g1( new GameObject( ) );
	game.addGameObject( g1 );
	game.run( );
	return 0;
}
