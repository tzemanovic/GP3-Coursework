#include "pch.h"
#include "game.h"
#include "controllerConfig.h"

// get random float in range [a, b]
const float randf( const float a, const float b );

void spawnAsteroid( Game& game, MeshModel& model, std::shared_ptr< GameObject > spaceship, FMOD::Sound* asteroidAudio, const glm::vec3& pos, const glm::vec3& vel,
	const float scale );

int main( )
{
	WindowConfig windowConfig;
	windowConfig.setWindowStyle( WindowStyle::Fullscreen );
	Game game( "GP3 coursework", windowConfig );

	// create new player view
	std::shared_ptr< PlayerView > view( new PlayerView( ) );
	// set light attributes
	view->setLightDirection( glm::normalize( glm::vec3( 1.0f, 0.5f, 0.5f ) ) );
	view->setAmbientLightColor( glm::vec3( 0.155f, 0.2113f, 0.22f ) );
	view->setDiffuseLightColor( glm::vec3( 1.0f, 0.9792f, 0.75f ) );
	// const values for third person controller
	const glm::vec3 thirdPersonCamOffset = glm::vec3( 0.0f, 1.0f, 5.0f );
	const glm::quat thirdPersonCamRot = glm::rotate( glm::quat( ), 0.0f, glm::vec3( 1.0f, 0.0f, 0.0f ) );
	// const values for bird's-eye controller
	const glm::vec3 birdsEyeCamOffset = glm::vec3( 0.0f, 15.0f, 12.0f );
	const glm::quat birdsEyeCamRot = glm::rotate( glm::quat( ), -HALF_PI + 1.0f, glm::vec3( 1.0f, 0.0f, 0.0f ) );
	// use third person controller on start-up
	view->setCameraOffset( thirdPersonCamOffset );
	view->setCameraRotation( thirdPersonCamRot );
	// setup a new controller
	std::shared_ptr< Controller > controller( new Controller( ) );
	// helper structs for holding controller properties
	ControllerConfig config;
	ControllerState state;
	// start at min speed
	state.speed = config.minSpeed;
	state.isBirdsEye = false;
	// define controller's logic in the update callback function
	controller->setOnUpdate( [&config, &state, &view, &thirdPersonCamOffset, &thirdPersonCamRot, &birdsEyeCamOffset, &birdsEyeCamRot]
		( const Controller& controller, std::shared_ptr< GameObject > gameObject, const Time& time )
	{
		float deltaS = time.deltaMs * 0.001f;
		// speed up
		if ( controller.getKeyState( Key::E ) )
		{
			state.speed += config.accel * deltaS;
		}
		// slow down
		if ( controller.getKeyState( Key::Q ) )
		{
			state.speed -= config.decel * deltaS;
		}
		// turn left
		if ( controller.getKeyState( Key::A ) )
		{
			state.turnRightLeftSpeed += config.turnRightLeftAccel * deltaS;
		}
		// turn right
		if ( controller.getKeyState( Key::D ) )
		{
			state.turnRightLeftSpeed -= config.turnRightLeftAccel * deltaS;
		}
		// if not turning left or right, gradually turn off rotation
		if ( !controller.getKeyState( Key::A ) && !controller.getKeyState( Key::D ) )
		{
			state.turnRightLeftSpeed -= config.turnRightLeftDecel * state.turnRightLeftSpeed;
		}
		// turn up
		if ( controller.getKeyState( Key::S ) )
		{
			state.turnUpDownSpeed += config.turnUpDownAccel * deltaS;
		}
		// turn down
		if ( controller.getKeyState( Key::W ) )
		{
			state.turnUpDownSpeed -= config.turnUpDownAccel * deltaS;
		}
		// if not turning up or down, gradually turn off rotation
		if ( !controller.getKeyState( Key::W ) && !controller.getKeyState( Key::S ) )
		{
			state.turnUpDownSpeed -= config.turnUpDownDecel * state.turnUpDownSpeed;
		}
		// switch between 3rd person and bird's-eye view
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
		// toggle all sounds
		if ( controller.getKeyState( Key::H ) && !controller.getOldKeyState( Key::H ) )
		{
			AudioManager::get( )->toggleSound( );
		}
		// clamp values with limits
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
		// gameObject is the listener, update AudioManager of its attributes
		AudioManager::get( )->updateListener( gameObject->getPos( ), velocity, forward, up );
	} );
	// use the controller we just defined
	view->setController( controller );
	game.addView( view );
	// create spaceship game object
	std::shared_ptr< GameObject > spaceship( new GameObject( ) );
	// this is the controller game object
	controller->setControlledObject( spaceship );
	spaceship->setPos( glm::vec3( 0.0f, -0.5f, -5.0f ) );
	spaceship->setScale( 0.003f );
	// load spaceship model
	MeshModel spaceshipModel( "assets/models/SpaceShuttleOrbiter/SpaceShuttleOrbiter.3ds", game );
	// fix its rotation
	spaceshipModel.rotateMesh( glm::vec3( 1.0f, 0.0f, 0.0f ), -HALF_PI + 0.1f );
	// create a component out of it
	std::shared_ptr< MeshComponent > spaceshipMesh( new MeshComponent( spaceshipModel ) );
	GameObject::addComponent( spaceship, spaceshipMesh );
	// spaceship sound, play on loop
	auto spaceshipAudio = AudioManager::get( )->loadSound( "assets/audio/spaceship.wav", false, true );
	std::shared_ptr< AudioComponent > spaceshipAudioComp( new AudioComponent( spaceshipAudio ) );
	spaceshipAudioComp->play( -1 );
	GameObject::addComponent( spaceship, spaceshipAudioComp );
	// add spaceship to the game
	game.addGameObject( spaceship );
	// spaceship is the owner of the view
	view->vOwn( spaceship );

	// preload asteroid models
	MeshModel asteroidModels[] = {
		MeshModel( "assets/models/asteroids/asteroid1.obj", game ),
		MeshModel( "assets/models/asteroids/asteroid2.obj", game ),
		MeshModel( "assets/models/asteroids/asteroid3.obj", game ),
		MeshModel( "assets/models/asteroids/asteroid4.obj", game ),
	};
	// load asteroid collision sound
	auto asteroidAudio = AudioManager::get( )->loadSound( "assets/audio/collision.wav", false );
	// generate 30 asteroids with random position, scale and velocity
	for ( unsigned i = 1; i <= 30; ++i )
	{
		const float posOffset = 100.0f;
		auto pos = glm::vec3( randf( -posOffset, posOffset ), randf( -posOffset, posOffset ), randf( -posOffset, posOffset ) );
		const static float velOffset = 0.05f;
		auto vel = glm::vec3( randf( -velOffset, velOffset ), randf( -velOffset, velOffset ), randf( -velOffset, velOffset ) );
		spawnAsteroid( game, asteroidModels[i % 4], spaceship, asteroidAudio, pos, vel, randf( 0.1f, 0.4f ) );
	}
	// create skybox
	std::shared_ptr< GameObject > skybox( new GameObject( ) );
	// make sure it's big enough
	skybox->setScale( 10.0f );
	SkyboxMeshModel skyboxModel( game );
	std::shared_ptr< MeshComponent > skyboxMesh( new MeshComponent( skyboxModel ) );
	GameObject::addComponent( skybox, skyboxMesh );
	game.addGameObject( skybox );
	// all setup is done, finally run the game
	game.run( );
	return 0;
}
const float randf( const float a, const float b )
{
	return ( ( b - a )*( (float) rand( ) / RAND_MAX ) ) + a;
}
void spawnAsteroid( Game& game, MeshModel& model, std::shared_ptr< GameObject > spaceship, FMOD::Sound* asteroidAudio, const glm::vec3& pos, const glm::vec3& vel,
	const float scale )
{
	std::shared_ptr< GameObject > asteroid( new GameObject( ) );
	asteroid->setPos( pos );
	asteroid->setScale( scale );
	// random rotation
	asteroid->rotate( glm::vec3( 1.0f, 1.0f, 0.0f ), randf( -PI, PI ) );
	// set collision timeout to 1 second
	asteroid->setCollisionTimeoutMs( 1000.0f );
	// prevent it from collision right after it's spawned
	asteroid->setJustCollided( );
	asteroid->setVelocity( vel );
	// prepare random rotation for the update callback function
	glm::vec3 rotAxis = glm::normalize( glm::vec3( randf( 0.0f, 1.0f ), randf( 0.0f, 1.0f ), randf( 0.0f, 1.0f ) ) );
	const float rotation = randf( -1.0f, 1.0f );
	// set asteroid update callback function
	asteroid->setOnUpdate( [&game, &model, spaceship, asteroidAudio, rotAxis, rotation]( GameObject& gameObject, const Time& time )
	{
		const float deltaS = time.deltaMs * 0.001f;
		// rotate the asteroid
		gameObject.rotate( rotAxis, rotation * deltaS );
		// check for collision with spaceship
		if ( spaceship->sphereSphereCollision( gameObject ) )
		{
			// check if the asteroid is not too small already
			const float scale = gameObject.getScale( ).x;
			if ( scale > 0.01f )
			{
				// play audio component on collision
				auto audio = gameObject.getComponent< AudioComponent >( ComponentType::Audio ).lock( );
				audio->play( 0, 0.6f );
				// scale down this asteroid in half
				gameObject.scale( 0.5f );
				// adjust its velocity slightly in direction of spaceship
				glm::vec3 spaceshipOrientedVelocity = glm::vec3( spaceship->getTransformNonScaled( ) * glm::vec4( spaceship->getVelocity( ), 0.0f ) );
				gameObject.setVelocity( gameObject.getVelocity( ) + spaceshipOrientedVelocity* 0.1f );
				// spawn a second asteroid of same size
				spawnAsteroid( game, model, spaceship, asteroidAudio, gameObject.getPos( ), gameObject.getVelocity( ) * -1.0f, scale * 0.5f );
			}
		}
	} );
	// add asteroid mesh and audio components
	std::shared_ptr< MeshComponent > asteroidMesh( new MeshComponent( model ) );
	GameObject::addComponent( asteroid, asteroidMesh );
	std::shared_ptr< AudioComponent > asteroidAudioComp( new AudioComponent( asteroidAudio ) );
	GameObject::addComponent( asteroid, asteroidAudioComp );
	// add asteroid to the game
	game.addGameObject( asteroid );
}