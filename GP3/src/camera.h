#pragma once

#include "sceneNode.h"

class Camera : public SceneNode
{
public:
	explicit Camera( const float aspectRatio );
public:
	const glm::mat4& getProjection( ) const
	{
		return _projection;
	}
	const glm::mat4& getView( ) const
	{
		return _fromWorld;
	}
private:
	glm::mat4 _projection;
};