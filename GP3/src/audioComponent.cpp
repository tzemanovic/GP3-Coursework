#include "pch.h"
#include "audioComponent.h"
#include "audioManager.h"
#include "gameObject.h"

AudioComponent::AudioComponent( FMOD::Sound* sound ) : Component( ComponentType::Audio ), _sound( sound )
{

}
AudioComponent::~AudioComponent( )
{
	if ( _channel != nullptr )
	{
		_channel->stop( );
		delete _channel;
		_channel = nullptr;
	}
	if ( _sound != nullptr )
	{
		_sound->release( );
		delete _sound;
		_sound = nullptr;
	}
}
void AudioComponent::play( const int loopCount, const float volume )
{
	if ( _sound != nullptr )
	{
		// only play if is not playing
		if ( _channel != nullptr )
		{
			bool isPlaying;
			_channel->isPlaying( &isPlaying );
			if ( isPlaying )
			{
				return;
			}
		}
		_channel = AudioManager::get( )->playSound( _sound, loopCount );
		_channel->setVolume( volume );
	}
}
void AudioComponent::vUpdate( const Time& time )
{
	if ( _sound != nullptr && _channel != nullptr )
	{
		// update channel's attributes
		const glm::vec3 position = _owner->getPos( );
		const glm::vec3 velocity = _owner->getVelocity( );
		FMOD_VECTOR pos{ position.x, position.y, position.z };
		FMOD_VECTOR vel{ velocity.x, velocity.y, velocity.z };
		_channel->set3DAttributes( &pos, &vel );
	}
}