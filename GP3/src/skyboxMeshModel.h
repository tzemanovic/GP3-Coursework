#pragma once

#include "meshModel.h"

class SkyboxMeshModel : public MeshModel
{
public:
	SkyboxMeshModel( const Game& game );
	virtual ~SkyboxMeshModel( );
public:
	virtual void vRender( const Scene& scene, const Time& time, std::shared_ptr< Camera > camera, const glm::mat4& toWorld ) override;
};