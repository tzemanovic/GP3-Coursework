#pragma once

#include "meshComponent.h"

class SkyboxMeshComponent : public MeshComponent
{
public:
	SkyboxMeshComponent( MeshModel& model );
	virtual ~SkyboxMeshComponent( );
public:
	virtual void vInit( Game& game ) override;
};