#pragma once

#include "componentType.h"

class GameObject;
class Game;
struct Time;

class Component
{
	friend class GameObject;
public:
	Component( ComponentType type );
	virtual ~Component( );
public:
	virtual void vInit( Game& game )
	{
	}
	virtual void vUpdate( const Time& time )
	{
	}
public:
	ComponentType getType( );
	std::shared_ptr< GameObject > getOwner( );
protected:
	void setOwner( std::shared_ptr< GameObject > owner );
protected:
	ComponentType					_type;
	std::shared_ptr< GameObject >	_owner;
};