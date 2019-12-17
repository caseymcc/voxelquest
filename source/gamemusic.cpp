#include "voxelquest/gamemusic.h"

#include <iostream>

GameMusic::GameMusic()
{

}

#ifdef VOXELQUEST_USE_SFML
void GameMusic::init(std::string path)
{
    if(!sound.openFromFile(path))
    {
		std::cout<<"error loading music";
        return;
    }

}

void GameMusic::setLoop(bool val)
{
    sound.setLoop(val);
}

void GameMusic::stop()
{
    sound.stop();
}

void GameMusic::setVolume(float _volume)
{
    volume=_volume;
    float intVol=_volume*100.0f;
    sound.setVolume(intVol);
}

void GameMusic::play(float _volume)
{
    setVolume(_volume);
    sound.play();
}
#else
void GameMusic::init(std::string path)
{
}

void GameMusic::setLoop(bool val)
{
}

void GameMusic::stop()
{
}

void GameMusic::setVolume(float _volume)
{
}

void GameMusic::play(float _volume)
{
}
#endif
