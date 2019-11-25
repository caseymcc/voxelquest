#ifndef _voxelquest_gamemusic_h_
#define _voxelquest_gamemusic_h_

class GameMusic
{
public:
    GameMusic()

        void init(string path)

        void setLoop(bool val)

        void stop()

        void setVolume(float _volume=1.0f)

        void play(float _volume=1.0f)
private:

    //sf::SoundBuffer buffer;
    sf::Music sound;

    float volume;
};

#endif//_voxelquest__h_