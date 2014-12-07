#include "pch.h"
#include "game.h"

int main( )
{
	WindowConfig windowConfig;
	Game game( L"GP3 coursework", windowConfig );
	game.run( );
	return 0;
}
