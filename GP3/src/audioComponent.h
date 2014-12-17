#pragma once

#include "component.h"

class AudioComponent : public Component
{
public:
	AudioComponent( FMOD::Sound* sound );
	~AudioComponent( );
public:
	// loopCount: Number of times to loop before stopping. 0 = oneshot. 1 = loop once then stop. -1 = loop forever. Default = 0
	// volume: [0.0f, 1.0f]
	void play( const int loopCount = 0, const float volume = 1.0f );
public:
	virtual void vUpdate( const Time& time ) override;
private:
	FMOD::Sound* 	_sound;
	FMOD::Channel* 	_channel;
	bool 			_looped;
};