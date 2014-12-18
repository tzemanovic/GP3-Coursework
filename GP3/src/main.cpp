#include "pch.h"
#include "game.h"
#include "controllerConfig.h"

float randf( float a, float b );
void spawnAsteroid( Game& game, MeshModel& model, std::shared_ptr< GameObject > spaceship, FMOD::Sound* asteroidAudio, const glm::vec3 pos, const glm::vec3 vel,
	const float scale );
int main( )
{
	WindowConfig windowConfig;
	windowConfig.setWindowStyle( WindowStyle::Fullscreen );
	Game game( "GP3 coursework", windowConfig );

	std::shared_ptr< PlayerView > view( new PlayerView( ) );
	view->setLightDirection( glm::normalize( glm::vec3( 1.0f, 0.5f, 0.5f ) ) );
	view->setAmbientLightColor( glm::vec3( 0.155f, 0.2113f, 0.22f ) );
	view->setDiffuseLightColor( glm::vec3( 1.0f, 0.9792f, 0.75f ) );
	const glm::vec3 thirdPersonCamOffset = glm::vec3( 0.0f, 1.0f, 5.0f );
	const glm::vec3 birdsEyeCamOffset = glm::vec3( 0.0f, 15.0f, 12.0f );
	const glm::quat thirdPersonCamRot = glm::rotate( glm::quat( ), 0.0f, glm::vec3( 1.0f, 0.0f, 0.0f ) );
	const glm::quat birdsEyeCamRot = glm::rotate( glm::quat( ), -HALF_PI + 1.0f, glm::vec3( 1.0f, 0.0f, 0.0f ) );
	view->setCameraOffset( thirdPersonCamOffset );
	view->setCameraRotation( thirdPersonCamRot );

	std::shared_ptr< Controller > controller( new Controller( ) );
	{
		ControllerConfig config;
		ControllerState state;
		state.speed = config.maxSpeed;
		state.isBirdsEye = false;
		controller->setOnUpdate( [&config, &state, &view, &thirdPersonCamOffset, &thirdPersonCamRot, &birdsEyeCamOffset, &birdsEyeCamRot]
			( const Controller& controller, std::shared_ptr< GameObject > gameObject, const Time& time )
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
			if ( controller.getKeyState( Key::G ) && !controller.getOldKeyState( Key::G ) )
			{
				if ( state.isBirdsEye )
				{
					view->setCameraOffset( thirdPersonCamOffset );
					view->setCameraRotation( thirdPersonCamRot );
				}
				else
				{
					view->setCameraOffset( birdsEyeCamOffset );
					view->setCameraRotation( birdsEyeCamRot );
				}
				state.isBirdsEye = !state.isBirdsEye;
			}
			if ( controller.getKeyState( Key::H ) && !controller.getOldKeyState( Key::H ) )
			{
				AudioManager::get( )->toggleSound( );
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
			glm::vec3 forward = glm::vec3( transform * glm::vec4( 0.0f, 0.0f, -1.0f, 0.0f ) );
			glm::vec3 velocity = forward * state.speed;
			glm::vec3 up = glm::vec3( transform * glm::vec4( 0.0f, 1.0f, 0.0f, 0.0f ) );
			gameObject->setVelocity( velocity );
			AudioManager::get( )->updateListener( gameObject->getPos( ), velocity, forward, up );
		} );
	}

	view->setController( controller );
	game.addView( view );

	std::shared_ptr< GameObject > spaceship( new GameObject( ) );
	controller->setControlledObject( spaceship );
	spaceship->setPos( glm::vec3( 0.0f, -0.5f, -5.0f ) );
	spaceship->setScale( 0.003f );
	MeshModel spaceshipModel( "assets/models/SpaceShuttleOrbiter/SpaceShuttleOrbiter.3ds", game );
	spaceshipModel.rotateMesh( glm::vec3( 1.0f, 0.0f, 0.0f ), -HALF_PI + 0.1f );
	std::shared_ptr< MeshComponent > spaceshipMesh( new MeshComponent( spaceshipModel ) );
	GameObject::addComponent( spaceship, spaceshipMesh );
	auto spaceshipAudio = AudioManager::get( )->loadSound( "assets/audio/spaceship.wav", false, true );
	std::shared_ptr< AudioComponent > spaceshipAudioComp( new AudioComponent( spaceshipAudio ) );
	spaceshipAudioComp->play( -1 );
	GameObject::addComponent( spaceship, spaceshipAudioComp );
	game.addGameObject( spaceship );

	view->vOwn( spaceship );

	MeshModel asteroidModels[] = {
		MeshModel( "assets/models/asteroids/asteroid1.obj", game ),
		MeshModel( "assets/models/asteroids/asteroid2.obj", game ),
		MeshModel( "assets/models/asteroids/asteroid3.obj", game ),
		MeshModel( "assets/models/asteroids/asteroid4.obj", game ),
	};
	auto asteroidAudio = AudioManager::get( )->loadSound( "assets/audio/collision.wav", false );
	std::vector<std::shared_ptr< GameObject > > asteroids;
	for ( unsigned i = 1; i <= 30; ++i )
	{
		const float posOffset = 100.0f;
		auto pos = glm::vec3( randf( -posOffset, posOffset ), randf( -posOffset, posOffset ), randf( -posOffset, posOffset ) );
		const static float velOffset = 1.0f;
		auto vel = glm::vec3( randf( -velOffset, velOffset ), randf( -velOffset, velOffset ), randf( -velOffset, velOffset ) );
		spawnAsteroid( game, asteroidModels[i % 4], spaceship, asteroidAudio, pos, vel, randf( 0.1f, 0.4f ) );
	}

	std::shared_ptr< GameObject > skybox( new GameObject( ) );
	skybox->setScale( 10.0f );
	SkyboxMeshModel skyboxModel( game );
	std::shared_ptr< MeshComponent > skyboxMesh( new MeshComponent( skyboxModel ) );
	GameObject::addComponent( skybox, skyboxMesh );
	game.addGameObject( skybox );

	game.run( );

	return 0;
}
float randf( float a, float b )
{
	return ( ( b - a )*( (float) rand( ) / RAND_MAX ) ) + a;
}
void spawnAsteroid( Game& game, MeshModel& model, std::shared_ptr< GameObject > spaceship, FMOD::Sound* asteroidAudio, const glm::vec3 pos, const glm::vec3 vel,
	const float scale )
{
	std::shared_ptr< GameObject > asteroid( new GameObject( ) );
	asteroid->setPos( pos );
	asteroid->setScale( scale );
	asteroid->rotate( glm::vec3( 1.0f, 1.0f, 0.0f ), randf( -PI, PI ) );
	asteroid->setCollisionTimeoutMs( 1000.0f );
	asteroid->setJustCollided( );
	asteroid->setVelocity( vel * -1.0f );
	glm::vec3 rotAxis = glm::normalize( glm::vec3( randf( 0.0f, 1.0f ), randf( 0.0f, 1.0f ), randf( 0.0f, 1.0f ) ) );
	const float rotation = randf( -1.0f, 1.0f );
	asteroid->setOnUpdate( [&game, &model, spaceship, asteroidAudio, rotAxis, rotation]( GameObject& gameObject, const Time& time )
	{
		const float deltaS = time.deltaMs * 0.001f;
		gameObject.rotate( rotAxis, rotation * deltaS );
		// check for collision with spaceship
		if ( spaceship->sphereSphereCollision( gameObject ) )
		{
			const float scale = gameObject.getScale( ).x;
			if ( scale > 0.01f )
			{
				auto audio = gameObject.getComponent< AudioComponent >( ComponentType::Audio ).lock( );
				audio->play( 0, 0.6f );
				gameObject.scale( 0.5f );
				gameObject.setVelocity( gameObject.getVelocity( ) + spaceship->getVelocity( ) * 0.1f );
				gameObject.setScale( gameObject.getScale( ).x * 0.5f );
				spawnAsteroid( game, model, spaceship, asteroidAudio, gameObject.getPos( ), gameObject.getVelocity( ), scale * 0.5f );
			}
		}
	} );
	std::shared_ptr< MeshComponent > asteroidMesh( new MeshComponent( model ) );
	GameObject::addComponent( asteroid, asteroidMesh );
	std::shared_ptr< AudioComponent > asteroidAudioComp( new AudioComponent( asteroidAudio ) );
	GameObject::addComponent( asteroid, asteroidAudioComp );
	game.addGameObject( asteroid );
}