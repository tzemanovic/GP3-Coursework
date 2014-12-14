#include "pch.h"
#include "gameObject.h"
#include "component.h"
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject( ) : _pos( ), _scale( 1.f ), _rot( ), _transform( ), _transformNonScaled( ), _fixed( false )
{

}
GameObject::~GameObject( )
{

}
const GameObjectId GameObject::getId( ) const
{
	return _id;
}
const glm::mat4& GameObject::getTransform( ) const
{
	return _transform;
}
const glm::mat4& GameObject::getTransformNonScaled( ) const
{
	return _transformNonScaled;
}
bool GameObject::isFixed( ) const
{
	return _fixed;
}
void GameObject::setFixed( bool fixed )
{
	_fixed = fixed;
}
template< class TComponent > std::weak_ptr< TComponent > GameObject::getComponent( ComponentType type )
{
	// return weak_ptr to avoid circular reference
	auto componentPair = _components.find( type );
	if ( componentPair != _components.end( ) )
	{
		return std::static_pointer_cast< TComponent >( componentPair->second );
	}
	return std::weak_ptr< TComponent >( );
}
void GameObject::update( const Time& time )
{
	if ( !_fixed )
	{
		_transform = glm::mat4_cast( _rot );
		_transformNonScaled = glm::translate( _transform, _pos );
		_transform = glm::scale( _transformNonScaled, _scale );
	}
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