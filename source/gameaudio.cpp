#include "voxelquest/gameaudio.h"
#include "voxelquest/gamesound.h"
#include "voxelquest/renderer.h"
#include "voxelquest/baseobject.h"
#include "voxelquest/entenums.h"
#include "voxelquest/bullethelpers.h"
#include "voxelquest/settings.h"
#include "voxelquest/gamestate.h"
#include "voxelquest/gamegui.h"
#include "voxelquest/gameworld.h"

GameMusic* GameAudio::music[EML_LENGTH];
std::map<std::string, GameSound> GameAudio::soundMap;

void GameAudio::init()
{
    std::string musicStrings[]={
        "birdsong0",
        "crickets0",
        "oceanwaves0",
        "underwater0"
    };

    for(int i=0; i<EML_LENGTH; i++)
    {
        music[i]=new GameMusic();
        music[i]->init("data\\music\\"+musicStrings[i]+".ogg");
        music[i]->setLoop(true);
    }
}

void GameAudio::prepSound(std::string soundName)
{
	if(soundMap.find(soundName)==soundMap.end())
	{
		soundMap.insert(std::pair<std::string, GameSound>(soundName, GameSound()));
		soundMap[soundName].init("data\\sounds\\"+soundName+".wav");
	}
}

void GameAudio::playSoundEnt(
	std::string soundName,
	BaseObj* ge,
	float variance,
	float volume,
	bool doLoop
)
{

	if(ge==NULL)
	{
		playSoundPosAndPitch(soundName, Renderer::cameraGetPosNoShake(), Renderer::cameraGetPosNoShake(), variance, volume, doLoop);
	}
	else
	{
		if(
			(ge->entType==E_ENTTYPE_TRACE)||
			(ge->isHidden)
			)
		{

		}
		else
		{
            FIVector4 geCenter=convertToVQV(ge->getCenterPoint(E_BDG_CENTER));

			playSoundPosAndPitch(soundName, Renderer::cameraGetPosNoShake(), &geCenter, variance, volume, doLoop);
		}
	}




}

void GameAudio::playSoundPosAndPitch(
	std::string soundName,
	FIVector4* listenerPos,
	FIVector4* soundPos,
	float variance,
	float volume,
	bool doLoop
)
{


	FIVector4 res;

	prepSound(soundName);

	res.setFXYZRef(soundPos);
	res.addXYZRef(listenerPos, -1.0f);

	soundMap[soundName].setPitch(
		(fGenRand()-0.5f)*2.0f*variance+1.0f
	);

	soundMap[soundName].setPositionAndMinDis(
		res.getFX(),
		res.getFY(),
		res.getFZ(),
		32.0f
	);

	soundMap[soundName].setLoop(doLoop);
	soundMap[soundName].play(volume*g_settings.fxVolume*g_settings.masterVolume);
}

void GameAudio::updateSoundPosAndPitch(
	std::string soundName,
	FIVector4* listenerPos,
	FIVector4* soundPos,
	float volume,
	float decay
)
{


	FIVector4 res;

	prepSound(soundName);

	res.setFXYZRef(soundPos);
	res.addXYZRef(listenerPos, -1.0f);


	soundMap[soundName].setPositionAndMinDis(
		res.getFX(),
		res.getFY(),
		res.getFZ(),
		32.0f
	);

	soundMap[soundName].setVolumeSmooth(volume, decay);

}



void GameAudio::playSound(std::string soundName, float volume)
{
	prepSound(soundName);
	soundMap[soundName].play(volume);
}

void GameAudio::playSoundEvent(const char* eventName, bool suppress)
{

	if(suppress)
	{
		return;
	}

	std::string tempString;
	float volume;

	if(GameState::ui!=NULL)
	{
		if(GameState::ui->isReady)
		{
			tempString=GameState::ui->jvSounds->Child(eventName)->Child("name")->string_value;
			volume=(float)GameState::ui->jvSounds->Child(eventName)->Child("vol")->number_value;

			if(tempString.size()>0)
			{
				playSound(tempString, g_settings.masterVolume*volume*g_settings.guiVolume);
			}
		}
	}


}

void GameAudio::updateAmbientSounds()
{
    int i;
    int j;

    int maxRad=2;

    float avgHeight=0.0f;
    float tot=0.0f;

    for(i=-maxRad; i<=maxRad; i++)
    {
        for(j=-maxRad; j<=maxRad; j++)
        {
            avgHeight+=GameState::gw->getHeightAtPixelPos(
                Renderer::cameraGetPosNoShake()->getFX()+i*256.0f,
                Renderer::cameraGetPosNoShake()->getFY()+j*256.0f
            );
            tot+=1.0f;
        }
    }

    float terHeight=avgHeight/tot;
    float seaHeight=GameState::gw->getSeaHeightScaled();
    float heightDif=clampf((terHeight-seaHeight)/(1024.0f), 0.0f, 1.0f);
    float isUnderWater=GameState::gw->getUnderWater();

    // if (cameraGetPosNoShake()->getFZ() < seaHeight) {
    // 	isUnderWater = 1.0f;	
    // }
    music[EML_BIRDSONG0]->setVolume(g_settings.masterVolume*g_settings.ambientVolume*GameState::gw->timeOfDay*heightDif*(1.0f-isUnderWater));
    music[EML_CRICKETS0]->setVolume(g_settings.masterVolume*g_settings.ambientVolume*(1.0f-GameState::gw->timeOfDay)*heightDif*(1.0f-isUnderWater));
    music[EML_OCEANWAVES0]->setVolume(g_settings.masterVolume*g_settings.ambientVolume*(1.0f-heightDif)*(1.0f-isUnderWater));
    music[EML_UNDERWATER0]->setVolume(g_settings.masterVolume*g_settings.ambientVolume*(isUnderWater));
}
