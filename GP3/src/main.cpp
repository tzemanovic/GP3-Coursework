#include "pch.h"
#include "windowConfig.h"
#include "window.h"

int main( )
{
	WindowConfig windowConfig;
	Window window( L"GP3 coursework", windowConfig );
	bool running = true;
	double startTime = Window::time( );
	double previousTime = 0.0;
	while ( running )
	{
		double currentTime = Window::time( ) - startTime;
		double fps = 1000 / ( currentTime - previousTime );
		std::cout << ( fps ) << '\n';
		previousTime = currentTime;
		InputMessage msg;
		while ( window.popMessage( msg ) )
		{
			switch ( msg.type )
			{
			case InputMessage::Type::Close:
			{
				window.close( );
				running = false;
				break;
			}
			case InputMessage::Type::KeyDown:
			{
				if ( msg.key.type == Key::F4 && msg.key.lAlt )
				{
					window.close( );
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
	}
	return 0;
}
