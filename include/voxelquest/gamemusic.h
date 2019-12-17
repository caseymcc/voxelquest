#ifndef _voxelquest_gamemusic_h_
#define _voxelquest_gamemusic_h_

#include <string>

#ifdef VOXELQUEST_USE_SFML
#include "SFML/Audio.hpp"
#endif

class GameMusic
{
public:
	GameMusic();

	void init(std::string path);

	void setLoop(bool val);

	void stop();

	void setVolume(float _volume=1.0f);

	void play(float _volume=1.0f);

private:

#ifdef VOXELQUEST_USE_SFML
    //sf::SoundBuffer buffer;
    sf::Music sound;
#endif

    float volume;
};

#endif//_voxelquest__h_