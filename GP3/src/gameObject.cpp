#include "pch.h"
#include "gameObject.h"
#include "component.h"
#include "meshComponent.h"
#include "time.h"
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject( ) : _pos( ), _scale( 1.f ), _rot( ), _transform( ), _transformNonScaled( ), _fixed( false ), _canCollide( true )
{

}
GameObject::~GameObject( )
{

}
void GameObject::init( Game& game )
{
	for each( auto component in _components )
	{
		component.second->vInit( game );
	}
}
void GameObject::update( const Time& time )
{
	// keep track of time of last collision to be able to prevent more collisions for a specified duration in _collisionTimeoutMs
	if ( _justCollided )
	{
		_lastCollisionMs = time.currentMs;
		_canCollide = false;
		_justCollided = false;
	}
	// time for collision timeout ran out, can collide again
	if ( !_canCollide && _lastCollisionMs + _collisionTimeoutMs < time.currentMs )
	{
		_canCollide = true;
	}
	// call update callback
	if ( _onUpdate != nullptr )
	{
		_onUpdate( *this, time );
	}
	// update transform
	if ( _fixed )
	{
		// fixed game object ignores velocity and rotation
		_transformNonScaled = glm::translate( glm::mat4( ), _pos );
		_transform = glm::scale( _transformNonScaled, _scale );
	}
	else
	{
		// dynamic game object transform calculations
		_pos += _velocity;
		_transformNonScaled = glm::translate( glm::mat4( ), _pos );
		_transformNonScaled = _transformNonScaled * glm::mat4_cast( _rot );
		_transform = glm::scale( _transformNonScaled, _scale );
	}
	// update components
	for each ( auto component in _components )
	{
		component.second->vUpdate( time );
	}
}
const bool GameObject::sphereSphereCollision( GameObject& other )
{
	// check if their distance is smaller than the sum of radii of their bounding spheres
	if ( _canCollide && other.canCollide( ) )
	{
		_justCollided = glm::distance( _pos, other.getPos( ) ) < getRadius( ) + other.getRadius( );
		other.setJustCollided( _justCollided );
		return _justCollided;
	}
	return false;
}
void GameObject::addComponent( std::shared_ptr< GameObject > gameObject, std::shared_ptr< Component > component )
{
	component->setOwner( gameObject );
	gameObject->addComponent( component );
}
void GameObject::setId( const GameObjectId id )
{
	_id = id;
}
void GameObject::addComponent( std::shared_ptr< Component > component )
{
	_components.insert( std::pair< ComponentType, std::shared_ptr< Component > >( component->getType( ), component ) );
}