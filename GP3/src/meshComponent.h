#pragma once

#include "renderComponent.h"
#include "vertex.h"

class MeshModel;

class MeshComponent : public RenderComponent
{
public:
	MeshComponent( MeshModel& model );
	virtual ~MeshComponent( );
public:
	virtual void vRender( const Scene& scene, const Time& time, std::shared_ptr< Camera > camera, const glm::mat4& toWorld ) override;
	virtual void vInit( Game& game ) override;
protected:
	MeshModel& _model;
};