#pragma once

#include "componentType.h"

class Component;
struct Time;

class GameObject
{
	friend class Game;
public:
	GameObject( );
	virtual ~GameObject( );
	GameObject( const GameObject& ) = delete;
	GameObject& operator=( const GameObject& ) = delete;
	void* operator new( size_t i )
	{
		// change alignment to 16-bit
		return _mm_malloc( i, 16 );
	}
	void operator delete( void* p )
	{
		_mm_free( p );
	}
public:
	const GameObjectId getId( ) const;
	const glm::mat4& getTransform( ) const;
	const glm::mat4& getTransformNonScaled( ) const;
	bool isFixed( ) const;
	void setFixed( bool fixed = true );
public:
	template< class TComponent > std::weak_ptr< TComponent > getComponent( ComponentType type );
	void update( const Time& time );
public:
	static void addComponent( std::shared_ptr< GameObject > gameObject, std::shared_ptr< Component > component );
private:
	void setId( const GameObjectId id );
	void addComponent( std::shared_ptr< Component > component );
private:
	glm::vec3												_pos, _scale;
	glm::quat												_rot;
	glm::mat4												_transform, _transformNonScaled;
	GameObjectId 											_id;
	bool													_fixed;
	std::vector< GameObject >								_children;
	std::map< ComponentType, std::shared_ptr< Component > > _components;
};