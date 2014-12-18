#include "pch.h"
#include "inputMessage.h"

// convenience operator overload to use Key as an array index
unsigned char operator+ ( Key val )
{
	return static_cast< unsigned char >( val );
}