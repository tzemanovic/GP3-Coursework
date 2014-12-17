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
void GameObject::init( Game& game )
{
	for each( auto component in _components )
	{
		component.second->vInit( game );
	}
}
void GameObject::update( const Time& time )
{
	if ( _onUpdate != nullptr )
	{
		_onUpdate( *this, time );
	}
	if ( !_fixed )
	{
		_transformNonScaled = glm::translate( glm::mat4( ), _pos );
		_transformNonScaled = _transformNonScaled * glm::mat4_cast( _rot );
		_transform = glm::scale( _transformNonScaled, _scale );
	}
	for each ( auto component in _components )
	{
		component.second->vUpdate( time );
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