#pragma once

#include "component.h"
#include "renderable.h"

class RenderComponent : public Component, Renderable
{
public:
	RenderComponent( ComponentType type );
public:
	virtual void vRender( const Time& time, OpenGl* openGl, std::shared_ptr< Camera > camera ) override;
};