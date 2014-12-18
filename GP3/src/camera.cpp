#include "pch.h"
#include "camera.h"
#include "gameObject.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera( ) : SceneNode( )
{
	_projection = glm::perspective( 45.0f, 1.833333333333333f, 0.001f, 1000.0f );
}
void Camera::setAspectRatio( const float aspectRatio )
{
	_projection = glm::perspective( 45.0f, aspectRatio, 0.001f, 1000.0f );
}
void Camera::vUpdate( const Scene& scene, const Time& time )
{
	if ( _target != nullptr )
	{
		_toWorld = glm::translate( _target->getTransformNonScaled( ), _offset ) * glm::mat4_cast( _rot );
		_fromWorld = glm::inverse( _toWorld );
	}
}