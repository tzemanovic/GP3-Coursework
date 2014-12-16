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
	const GameObjectId getId( ) const
	{
		return _id;
	}
	const glm::mat4& getTransform( ) const
	{
		return _transform;
	}
	const glm::mat4& getTransformNonScaled( ) const
	{
		return _transformNonScaled;
	}
	bool isFixed( ) const
	{
		return _fixed;
	}
	void setFixed( bool fixed = true )
	{
		_fixed = fixed;
	}
	void translate( const glm::vec3& translation )
	{
		_pos += translation;
	}
	const glm::vec3& getPos( ) const
	{
		return _pos;
	}
	void setPos( const glm::vec3& pos )
	{
		_pos = pos;
	}
	void setX( const float x )
	{
		_pos.x = x;
	}
	void setY( const float y )
	{
		_pos.y = y;
	}
	void setZ( const float z )
	{
		_pos.z = z;
	}
	void rotate( const glm::vec3& axis, const float angle )
	{
		_rot = glm::rotate( _rot, angle, axis );
	}
	void setOnUpdate( std::function< void( GameObject&, const Time& ) > onUpdate )
	{
		_onUpdate = onUpdate;
	}
public:
	template< class TComponent > std::weak_ptr< TComponent > getComponent( ComponentType type );
	void init( Game& game );
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
	std::function< void( GameObject&, const Time& ) > 		_onUpdate;
	std::vector< GameObject >								_children;
	std::map< ComponentType, std::shared_ptr< Component > > _components;
};