#pragma once

#include "componentType.h"

class Component;
struct Time;

class GameObject
{
	typedef std::function< void( GameObject&, const Time& ) > OnUpdate;
	typedef std::map< ComponentType, std::shared_ptr< Component > > ComponentMap;
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
	/*void translate( const glm::vec3& translation )
	{
	_velocity += translation;
	_pos += translation;
	}
	void translate( const glm::vec4& translation )
	{
	glm::vec3 trans( translation );
	_velocity += trans;
	_pos += trans;
	}*/
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
	void setVelocity( const glm::vec3& velocity )
	{
		_velocity = velocity;
	}
	const glm::vec3& getVelocity( ) const
	{
		return _velocity;
	}
	const glm::vec3& getScale( ) const
	{
		return _scale;
	}
	void setScale( const glm::vec3& scale )
	{
		_scale = scale;
	}
	void setScale( const float scale )
	{
		_scale = glm::vec3( scale );
	}
	void scale( const float scale )
	{
		_radius *= scale;
		_scale *= scale;
	}
	void rotate( const glm::vec3& axis, const float angle )
	{
		_rot = glm::rotate( _rot, angle, axis );
	}
	const float getRadius( ) const
	{
		return _radius;
	}
	void setRadius( const float radius )
	{
		_radius = radius;
	}
	void setOnUpdate( OnUpdate onUpdate )
	{
		_onUpdate = onUpdate;
	}
	void setJustCollided( const bool collided = true )
	{
		_justCollided = collided;
	}
	bool canCollide( ) const
	{
		return _canCollide;
	}
	void setCollisionTimeoutMs( const float timeout )
	{
		_collisionTimeoutMs = timeout;
	}
	template< class TComponent > std::weak_ptr< TComponent > getComponent( ComponentType type )
	{
		// return weak_ptr to avoid circular reference
		auto componentPair = _components.find( type );
		if ( componentPair != _components.end( ) )
		{
			return std::static_pointer_cast<TComponent>( componentPair->second );
		}
		return std::weak_ptr< TComponent >( );
	}
public:
	void init( Game& game );
	void update( const Time& time );
	const bool sphereSphereCollision( GameObject& other );
public:
	static void addComponent( std::shared_ptr< GameObject > gameObject, std::shared_ptr< Component > component );
private:
	void setId( const GameObjectId id );
	void addComponent( std::shared_ptr< Component > component );
private:
	glm::vec3		_pos, _scale, _velocity;
	glm::quat		_rot;
	glm::mat4		_transform, _transformNonScaled;
	float 			_radius;
	GameObjectId 	_id;
	bool			_fixed;
	OnUpdate		_onUpdate;
	ComponentMap 	_components;
	bool 			_justCollided;
	bool 			_canCollide;
	float 			_lastCollisionMs;
	float 			_collisionTimeoutMs;
};