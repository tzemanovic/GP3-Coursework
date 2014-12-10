#include "pch.h"
#include "renderComponent.h"
#include "scene.h"

RenderComponent::RenderComponent( ComponentType type ) : Component( type ), Renderable( )
{
	//scene.addSceneNode( RenderPassType::GameObject );
}
void RenderComponent::vRender( const Time& time, OpenGl* openGl, std::shared_ptr< Camera > camera )
{

}