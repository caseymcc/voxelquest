#ifndef _voxelquest_gameaudio_h_
#define _voxelquest_gameaudio_h_

#include "voxelquest/gamemusic.h"
#include "voxelquest/gamesound.h"

class GameAudio
{
    void prepSound(string soundName);

    void playSoundEnt(
        string soundName,
        BaseObj* ge=NULL,
        float variance=0.0f,
        float volume=1.0f,
        bool doLoop=false
    );

    void playSoundPosAndPitch(
        string soundName,
        FIVector4* listenerPos,
        FIVector4* soundPos,
        float variance=0.0f,
        float volume=1.0f,
        bool doLoop=false
    );

    void updateSoundPosAndPitch(
        string soundName,
        FIVector4* listenerPos,
        FIVector4* soundPos,
        float volume=1.0f,
        float decay=0.01f
    );

    void playSound(string soundName, float volume=1.0f);

    void playSoundEvent(const char* eventName, bool suppress=false);

    void updateMatVol();

    GameMusic* music[EML_LENGTH];
    map<string, GameSound> soundMap;
};

#endif//_voxelquest_gameaudio_h_