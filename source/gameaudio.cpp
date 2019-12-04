#include "voxelquest/gameaudio.h"
#include "voxelquest/gamesound.h"
#include "voxelquest/renderer.h"
#include "voxelquest/baseobject.h"
#include "voxelquest/entenums.h"
#include "voxelquest/bullethelpers.h"
#include "voxelquest/settings.h"
#include "voxelquest/gamestate.h"
#include "voxelquest/gamegui.h"

GameMusic* GameAudio::music[EML_LENGTH];
std::map<std::string, GameSound> GameAudio::soundMap;

void GameAudio::prepSound(std::string soundName)
{
	if(soundMap.find(soundName)==soundMap.end())
	{
		soundMap.insert(std::pair<std::string, GameSound>(soundName, GameSound()));
		soundMap[soundName].init("..\\data\\sounds\\"+soundName+".wav");
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


