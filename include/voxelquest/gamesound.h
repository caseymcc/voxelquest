#ifndef _voxelquest_gamesound_h_
#define _voxelquest_gamesound_h_

#include <string>

#ifdef VOXELQUEST_USE_SFML
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#endif

class GameSound
{
public:
	GameSound();

	void init(std::string path);

	void setPitch(float val);

	void setPositionAndMinDis(
		float x,
		float y,
		float z,
		float w
	);

	void setLoop(bool val);

	void stop();

	void setVolumeSmooth(float volume=1.0f, float decay=0.01f);


	void setVolume(float volume=1.0f);


	void play(float volume=1.0f);

private:

#ifdef VOXELQUEST_USE_SFML
	sf::SoundBuffer buffer;
	sf::Sound sound;
#endif

	float curVolume;
};

#endif//_voxelquest__h_