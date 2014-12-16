#pragma once

#include "component.h"
#include "renderable.h"

class RenderComponent : public Component, public Renderable
{
public:
	RenderComponent( ComponentType type );
};