#pragma once

class Camera;
class Scene;
struct Time;

class Renderable
{
public:
	Renderable( )
	{
	}
	virtual ~Renderable( )
	{
	}
public:
	virtual void vRender( const Scene& scene, const Time& time, std::shared_ptr< Camera > camera, const glm::mat4& toWorld = glm::mat4( ) ) = 0;
};