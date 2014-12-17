#include "pch.h"
#include "audioManager.h"

AudioManager::AudioManager( ) : _audioSystem( nullptr )
{
	// This setup is directly taken from the Getting Started with FMOD for Windows
	// Page 8 titled - Recommended startup sequence (IMPORTANT!)
	// temp values
	FMOD_RESULT result;
	// version of FMD
	unsigned int version;
	// number of audio drivers
	int numdrivers;
	// speaker mode
	FMOD_SPEAKERMODE speakermode;
	// The capabilities of the audio device
	FMOD_CAPS caps;
	// holds the name of the driver
	char name[256];
	// Create audio system
	result = FMOD::System_Create( &_audioSystem );
	// check version
	result = _audioSystem->getVersion( &version );
	if ( version < FMOD_VERSION )
	{
		ERR( "Wrong FMOD version" );
	}
	// Get number of drivers
	result = _audioSystem->getNumDrivers( &numdrivers );
	if ( numdrivers == 0 )
	{
		result = _audioSystem->setOutput( FMOD_OUTPUTTYPE_NOSOUND );
	}
	else
	{
		// Get driver caps and speaker mode, this will get the setup of the speakers from the OS
		result = _audioSystem->getDriverCaps( 0, &caps, 0, &speakermode );
		result = _audioSystem->setSpeakerMode( speakermode );
		if ( caps & FMOD_CAPS_HARDWARE_EMULATED )
		{
			/*
			The user has the 'Acceleration' slider set to off! This is really bad
			for latency! You might want to warn the user about this.
			*/
			result = _audioSystem->setDSPBufferSize( 1024, 10 );
		}
		// get the driver name
		result = _audioSystem->getDriverInfo( 0, name, 256, 0 );
		if ( strstr( name, "SigmaTel" ) )
		{
			/*
			Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
			PCM floating point output seems to solve it.
			*/
			result = _audioSystem->setSoftwareFormat( 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0,
				FMOD_DSP_RESAMPLER_LINEAR );
		}
		// finally init FMOD
		result = _audioSystem->init( 100, FMOD_INIT_NORMAL, 0 );
		if ( result == FMOD_ERR_OUTPUT_CREATEBUFFER )
		{
			/*
			Ok, the speaker mode selected isn't supported by this soundcard. Switch it
			back to stereo...
			*/
			result = _audioSystem->setSpeakerMode( FMOD_SPEAKERMODE_STEREO );
			/*
			... and re-init.
			*/
			result = _audioSystem->init( 100, FMOD_INIT_NORMAL, 0 );
		}
	}
}
AudioManager::~AudioManager( )
{
	if ( _audioSystem != nullptr )
	{
		_audioSystem->release( );
		_audioSystem = nullptr;
	}
}
void AudioManager::update( )
{
	if ( _audioSystem != nullptr )
	{
		_audioSystem->update( );
	}
}
// Load sound, should it be a streaming sound
FMOD::Sound* AudioManager::loadSound( String&& filename, bool stream )
{
	// load or stream sound
	FMOD_RESULT result;
	FMOD::Sound* sound;
	FMOD_MODE mode = FMOD_DEFAULT;
	if ( stream )
	{
		result = _audioSystem->createSound( filename.c_str( ), mode, 0, &sound );
	}
	else
	{
		result = _audioSystem->createStream( filename.c_str( ), mode, 0, &sound );
	}
	return sound;
}
FMOD::Channel* AudioManager::playSound( FMOD::Sound* sound, const int loopCount )
{
	// play sound, return a channel so we can control the sound while its playing
	FMOD::Channel* channel;
	_audioSystem->playSound( FMOD_CHANNEL_FREE, sound, false, &channel );
	channel->setLoopCount( loopCount );
	return channel;
}
void AudioManager::updateListener( const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up )
{
	if ( _audioSystem != nullptr )
	{
		FMOD_VECTOR pos{ position.x, position.y, position.z };
		FMOD_VECTOR vel{ velocity.x, velocity.y, velocity.z };
		FMOD_VECTOR forw{ forward.x, forward.y, forward.z };
		FMOD_VECTOR upw{ up.x, up.y, up.z };
		// update the listener's position and velocity
		_audioSystem->set3DListenerAttributes( 0, &pos, &vel, &forw, &upw );
	}
}