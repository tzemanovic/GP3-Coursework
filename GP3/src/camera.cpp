#include "pch.h"
#include "camera.h"
#include "gameObject.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(  ) : SceneNode( )
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
		glm::mat4 targetTransform = _target->getTransformNonScaled( );
		_toWorld = glm::translate( targetTransform, _offset );
		_fromWorld = glm::inverse( _toWorld );
/*
		fVec4 atWorld = targetMat.transform( _camOffset );
		fVec3 pos = mat.GetPosition( ) + fVec3( atWorld.x, atWorld.y, atWorld.z );
		mat.SetPosition( pos );
		SetTransform( &mat );*/
		/*Mat4x4 mat = m_pTarget->GetToWorldNoScale();
		fVec4 atWorld = mat.Transform( m_camOffset );
		fVec3 target = mat.GetPosition();
		fVec3 pos = target + fVec3( atWorld.x, atWorld.y, atWorld.z );
		Mat4x4 newTransform;
		Geometry::MatrixLookAtLH( &newTransform, pos, target, fVec3::up );
		SetTransform( &newTransform );*/
	}
}