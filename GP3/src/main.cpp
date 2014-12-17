#include "pch.h"
#include "game.h"
#include "controllerConfig.h"

float randf( float a, float b )
{
	return ( ( b - a )*( ( float ) rand( ) / RAND_MAX ) ) + a;
}

int main( )
{
	WindowConfig windowConfig;
	Game game( "GP3 coursework", windowConfig );

	std::shared_ptr< PlayerView > view( new PlayerView( ) );
	view->setLightDirection( glm::normalize( glm::vec3( 1.0f, 0.5f, 0.5f ) ) );
	view->setAmbientLightColor( glm::vec3( 0.075f, 0.1113f, 0.12f ) );
	view->setDiffuseLightColor( glm::vec3( 1.0f, 0.9792f, 0.75f ) );
	view->setCameraOffset( glm::vec3( 0.0f, 1.0f, 5.0f ) );

	std::shared_ptr< Controller > controller( new Controller( ) );
	{
		ControllerConfig config;
		ControllerState state;
		state.speed = config.maxSpeed;
		controller->setOnUpdate( [&config, &state]( const Controller& controller, std::shared_ptr< GameObject > gameObject, const Time& time )
		{
			float deltaS = time.deltaMs * 0.001f;
			if ( controller.getKeyState( Key::E ) )
			{
				state.speed += config.accel * deltaS;
			}
			if ( controller.getKeyState( Key::Q ) )
			{
				state.speed -= config.decel * deltaS;
			}
			if ( controller.getKeyState( Key::A ) )
			{
				state.turnRightLeftSpeed += config.turnRightLeftAccel * deltaS;
			}
			if ( controller.getKeyState( Key::D ) )
			{
				state.turnRightLeftSpeed -= config.turnRightLeftAccel * deltaS;
			}
			if ( !controller.getKeyState( Key::A ) && !controller.getKeyState( Key::D ) )
			{
				state.turnRightLeftSpeed -= config.turnRightLeftDecel * state.turnRightLeftSpeed;
			}
			if ( controller.getKeyState( Key::S ) )
			{
				state.turnUpDownSpeed += config.turnUpDownAccel * deltaS;
			}
			if ( controller.getKeyState( Key::W ) )
			{
				state.turnUpDownSpeed -= config.turnUpDownAccel * deltaS;
			}
			if ( !controller.getKeyState( Key::W ) && !controller.getKeyState( Key::S ) )
			{
				state.turnUpDownSpeed -= config.turnUpDownDecel * state.turnUpDownSpeed;
			}
			// clamp values
			state.speed = glm::clamp( state.speed, config.minSpeed, config.maxSpeed );
			state.turnRightLeftSpeed = glm::clamp( state.turnRightLeftSpeed, -config.maxTurnRightLeftSpeed, config.maxTurnRightLeftSpeed );
			state.turnUpDownSpeed = glm::clamp( state.turnUpDownSpeed, -config.maxTurnUpDownSpeed, config.maxTurnUpDownSpeed );
			// apply transformations
			gameObject->rotate( glm::vec3( 0.0f, 1.0f, 0.0f ), state.turnRightLeftSpeed * deltaS );
			gameObject->rotate( glm::vec3( 1.0f, 0.0f, 0.0f ), state.turnUpDownSpeed * deltaS );
			// multiply translation vector with object's transform matrix to apply the translation in a right direction
			glm::mat4 transform = gameObject->getTransformNonScaled( );
			glm::vec4 forward = transform * glm::vec4( 0.0f, 0.0f, -state.speed * deltaS, 0.0f );
			gameObject->translate( forward );
		} );
	}

	view->setController( controller );
	game.addView( view );

	std::shared_ptr< GameObject > g1( new GameObject( ) );
	controller->setControlledObject( g1 );
	g1->translate( glm::vec3( 0.0f, -0.5f, -5.0f ) );
	g1->setScale( 0.003f );
	MeshModel g1model( "assets/models/SpaceShuttleOrbiter/SpaceShuttleOrbiter.3ds", game );
	g1model.rotateMesh( glm::vec3( 1.0f, 0.0f, 0.0f ), -HALF_PI + 0.1f );
	std::shared_ptr<MeshComponent> g1mesh( new MeshComponent( g1model ) );
	GameObject::addComponent( g1, g1mesh );
	game.addGameObject( g1 );

	view->vOwn( g1 );


	MeshModel asteroidModels[] = {
		MeshModel( "assets/models/asteroids/asteroid1.obj", game ),
		MeshModel( "assets/models/asteroids/asteroid2.obj", game ),
		MeshModel( "assets/models/asteroids/asteroid3.obj", game ),
		MeshModel( "assets/models/asteroids/asteroid4.obj", game ),
	};
	for ( unsigned i = 1; i <= 20; ++i )
	{
		std::shared_ptr< GameObject > asteroid( new GameObject( ) );
		float offset = static_cast< float >( i );
		asteroid->translate( glm::vec3( randf( -50.0f, 50.0f ), randf( -50.0f, 50.0f ), randf( -50.0f, 50.0f ) ) );
		asteroid->setScale( randf( 0.04f, 0.1f ) );
		asteroid->rotate( glm::vec3( 1.0f, 1.0f, 0.0f ), randf( -PI, PI ) );
		glm::vec3 movement( randf( -5.0f, 5.0f ), randf( -5.0f, 5.0f ), randf( -5.0f, 5.0f ) );
		glm::vec3 rotAxis = glm::normalize( glm::vec3( randf( 0.0f, 1.0f ), randf( 0.0f, 1.0f ), randf( 0.0f, 1.0f ) ) );
		float rotation = randf( -1.0f, 1.0f );
		asteroid->setOnUpdate( [&movement, &rotAxis, rotation]( GameObject& gameObject, const Time& time )
		{
			float deltaS = time.deltaMs * 0.001f;
			gameObject.rotate( rotAxis, rotation * deltaS );
			gameObject.translate( movement * deltaS );
		} );
		std::shared_ptr<MeshComponent> asteroidMesh( new MeshComponent( asteroidModels[i % 4] ) );
		GameObject::addComponent( asteroid, asteroidMesh );
		game.addGameObject( asteroid );
		LOG( i );
	}

	game.run( );

	return 0;
}