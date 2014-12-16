#include "pch.h"
#include "renderPassType.h"

int operator+ ( RenderPassType val )
{
	return static_cast< unsigned >( val );
}
RenderPassType& operator++ ( RenderPassType &val )
{
	return val = static_cast< RenderPassType >( +val + 1 );
}