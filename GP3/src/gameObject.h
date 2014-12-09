#pragma once

class Component;

enum class ComponentType : unsigned
{
	Transform,
	Mesh,
	Count
};

class GameObject
{
public:
	GameObject( );
	virtual ~GameObject( );
	GameObject( const GameObject& ) = delete;
	GameObject& operator=( const GameObject& ) = delete;
private:
	std::map< ComponentType, std::shared_ptr< Component > > _components;
	glm::vec3												_position;
};