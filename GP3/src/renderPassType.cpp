#include "pch.h"
#include "renderPassType.h"

// convenience operator overload to use RenderPassType as an array index
int operator+ ( RenderPassType val )
{
	return static_cast< unsigned >( val );
}
// useful for looping through all the render pass types
RenderPassType& operator++ ( RenderPassType &val )
{
	return val = static_cast< RenderPassType >( +val + 1 );
}