#pragma once

#include "renderComponent.h"

class Scene;
class GLMmodel;

class MeshComponent : public RenderComponent
{
public:
	MeshComponent( Scene& scene );
protected:
	GLMmodel* _model;
};