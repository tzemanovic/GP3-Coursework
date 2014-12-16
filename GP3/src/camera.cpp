#include "pch.h"
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera( const float aspectRatio ) : SceneNode( )
{
	_projection = glm::perspective( 45.0f, aspectRatio, 0.001f, 1000.0f );
}