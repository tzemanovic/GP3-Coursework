#pragma once

enum class RenderPassType : unsigned
{
	First = 0,
	Sky,
	GameObject,
	HUD,
	Count
};

// covert to int to use as array index
int operator+ ( RenderPassType val );
// overload ++ to increment
RenderPassType& operator++ ( RenderPassType &val );