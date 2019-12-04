#ifndef _voxelquest_gamesound_h_
#define _voxelquest_gamesound_h_

#include <string>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

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

	sf::SoundBuffer buffer;
	sf::Sound sound;

	float curVolume;
};

#endif//_voxelquest__h_