#include "voxelquest/gameaudio.h"

GameMusic* GameAudio::music[EML_LENGTH];
std::map<std::string, GameSound> GameAudio::soundMap;

void GameAudio::prepSound(string soundName)
{
	if(soundMap.find(soundName)==soundMap.end())
	{
		soundMap.insert(pair<string, GameSound>(soundName, GameSound()));
		soundMap[soundName].init("..\\data\\sounds\\"+soundName+".wav");
	}
}

void GameAudio::playSoundEnt(
	string soundName,
	BaseObj* ge=NULL,
	float variance=0.0f,
	float volume=1.0f,
	bool doLoop=false
)
{

	if(ge==NULL)
	{
		playSoundPosAndPitch(soundName, cameraGetPosNoShake(), cameraGetPosNoShake(), variance, volume, doLoop);
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
			playSoundPosAndPitch(soundName, cameraGetPosNoShake(), BTV2FIV(ge->getCenterPoint(E_BDG_CENTER)), variance, volume, doLoop);
		}
	}




}

void GameAudio::playSoundPosAndPitch(
	string soundName,
	FIVector4* listenerPos,
	FIVector4* soundPos,
	float variance=0.0f,
	float volume=1.0f,
	bool doLoop=false
)
{


	FIVector4 res;

	prepSound(soundName);

	res.setFXYZRef(soundPos);
	res.addXYZRef(listenerPos, -1.0f);

	soundMap[soundName].setPitch(
		(fGenRand()-0.5f)*2.0*variance+1.0f
	);

	soundMap[soundName].setPositionAndMinDis(
		res.getFX(),
		res.getFY(),
		res.getFZ(),
		32.0f
	);

	soundMap[soundName].setLoop(doLoop);
	soundMap[soundName].play(volume*fxVolume*masterVolume);
}

void GameAudio::updateSoundPosAndPitch(
	string soundName,
	FIVector4* listenerPos,
	FIVector4* soundPos,
	float volume=1.0f,
	float decay=0.01f
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



void GameAudio::playSound(string soundName, float volume=1.0f)
{
	prepSound(soundName);
	soundMap[soundName].play(volume);
}

void GameAudio::playSoundEvent(const char* eventName, bool suppress=false)
{

	if(suppress)
	{
		return;
	}

	string tempString;
	float volume;

	if(mainGUI!=NULL)
	{
		if(mainGUI->isReady)
		{
			tempString=mainGUI->jvSounds->Child(eventName)->Child("name")->string_value;
			volume=mainGUI->jvSounds->Child(eventName)->Child("vol")->number_value;

			if(tempString.size()>0)
			{
				playSound(tempString, masterVolume*volume*guiVolume);
			}
		}
	}


}


