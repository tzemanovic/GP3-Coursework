#include "pch.h"
#include "component.h"

Component::Component( ComponentType type ) : _type( type )
{

}
Component::~Component( )
{
	_owner.reset( );
}
ComponentType Component::getType( )
{
	return _type;
}
std::shared_ptr< GameObject > Component::getOwner( )
{
	return _owner;
}
void Component::setOwner( std::shared_ptr< GameObject > owner )
{
	_owner = owner;
}