#pragma once

#include "sceneNode.h"

class GameObject;

class Camera : public SceneNode
{
public:
	Camera( );
public:
	void setAspectRatio( const float aspectRatio );
public:
	virtual void vUpdate( const Scene& scene, const Time& time ) override;
public:
	const glm::mat4& getProjection( ) const
	{
		return _projection;
	}
	const glm::mat4& getView( ) const
	{
		return _fromWorld;
	}
	void setTarget( std::shared_ptr< GameObject > target )
	{
		_target = target;
	}
	void setOffset( const glm::vec3& offset )
	{
		_offset = offset;
	}
	void setRotation( const glm::quat& rot )
	{
		_rot = rot;
	}
private:
	glm::mat4 						_projection;
	glm::vec3 						_offset;
	glm::quat 						_rot;
	std::shared_ptr< GameObject > 	_target;
};