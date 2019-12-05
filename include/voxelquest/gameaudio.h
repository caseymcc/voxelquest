#ifndef _voxelquest_gameaudio_h_
#define _voxelquest_gameaudio_h_

#include "voxelquest/gamemusic.h"
#include "voxelquest/gamesound.h"
#include "voxelquest/vectors.h"

#include <string>
#include <map>

enum E_MUSIC_LIST
{
    EML_BIRDSONG0,
    EML_CRICKETS0,
    EML_OCEANWAVES0,
    EML_UNDERWATER0,
    EML_LENGTH
};

class BaseObj;

class GameAudio
{
public:
    static void init();

    static void prepSound(std::string soundName);

    static void playSoundEnt(
        std::string soundName,
        BaseObj* ge=NULL,
        float variance=0.0f,
        float volume=1.0f,
        bool doLoop=false
    );

    static void playSoundPosAndPitch(
        std::string soundName,
        FIVector4* listenerPos,
        FIVector4* soundPos,
        float variance=0.0f,
        float volume=1.0f,
        bool doLoop=false
    );

    static void updateSoundPosAndPitch(
        std::string soundName,
        FIVector4* listenerPos,
        FIVector4* soundPos,
        float volume=1.0f,
        float decay=0.01f
    );

    static void playSound(std::string soundName, float volume=1.0f);

    static void playSoundEvent(const char* eventName, bool suppress=false);

    static void updateAmbientSounds();

    static GameMusic* music[EML_LENGTH];
    static std::map<std::string, GameSound> soundMap;
};

#endif//_voxelquest_gameaudio_h_