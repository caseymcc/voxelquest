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


void GameAudio::updateMatVol()
{



	matVolLock=true;

	JSONValue* jv=fetchJSONData("materials.js", false);
	JSONValue* curJV=NULL;
	JSONValue* curK=NULL;
	JSONValue* curJ=NULL;
	JSONValue* curI=NULL;
	JSONValue* curCol=NULL;

	int i;
	int j;
	int k;
	int m;
	int n;
	int p;
	int totN;

	int curInd0;
	int curInd0Prev;
	int curInd0Next;
	int curInd1;

	int mvInd;

	uint rv, gv, bv;

	joi_type itI;
	joi_type itJ;
	joi_type itK;

	float totRatio;
	float curLerp;
	float curLerpNoPower;
	float curPower;
	float curLerpWithPower;
	float curRatio;

	int mvx=matVolDim.getIX();
	int mvy=matVolDim.getIY();
	int mvz=matVolDim.getIZ();

	if(jv!=NULL)
	{





		curJV=jv->Child("materials");

		k=0;
		for(itK=curJV->object_value.begin(); itK!=curJV->object_value.end(); itK++)
		{
			curK=itK->second;

			j=0;
			for(itJ=curK->object_value.begin(); itJ!=curK->object_value.end(); itJ++)
			{
				curJ=itJ->second;

				i=0;
				totRatio=0.0f;
				for(itI=curJ->object_value.begin(); itI!=curJ->object_value.end(); itI++)
				{
					curI=itI->second;

					curInd0=i+j*mvx;
					curCol=curI->Child("i0_color");


					matSlice0[curInd0].h=curCol->Child(0)->number_value;
					matSlice0[curInd0].s=curCol->Child(1)->number_value;
					matSlice0[curInd0].l=curCol->Child(2)->number_value;

					hsv2rgb(&(matSlice0[curInd0]));

					matSlice0[curInd0].power=curI->Child("i1_power")->number_value;
					matSlice0[curInd0].ratio=curI->Child("i2_ratio")->number_value;

					if(matSlice0[curInd0].ratio<=0.0f)
					{
						matSlice0[curInd0].ratio=1.0f/(mvx-1.0f);
					}

					totRatio+=matSlice0[curInd0].ratio;

					i++;
				}


				if(i==0)
				{

				}
				else
				{
					for(m=0; m<i; m++)
					{
						curInd0=m+j*mvx;
						matSlice0[curInd0].ratio=matSlice0[curInd0].ratio*(matVolDim.getFX()-1.0f)/totRatio;
					}


					totN=0;
					for(m=0; m<i; m++)
					{
						curInd0=m+j*mvx;

						if(i==1)
						{
							curInd0Prev=curInd0;
							curInd0Next=curInd0;
						}
						else
						{
							curInd0Prev=(m-1)+j*mvx;
							curInd0Next=(m+1)+j*mvx;
						}



						curRatio=matSlice0[curInd0].ratio;

						for(n=0; (n<(curRatio+0.1f))&&(totN<mvx); n++)
						{
							curLerp=((float)n)/curRatio;
							curInd1=totN+j*mvx;

							if(m==0)
							{

								curLerpNoPower=curLerp*0.5f;
								curPower=mixf(matSlice0[curInd0].power, matSlice0[curInd0Next].power, curLerpNoPower);
								curLerpWithPower=pow(curLerpNoPower, curPower*8.0f);

								matSlice1[curInd1].r=mixf(matSlice0[curInd0].r, matSlice0[curInd0Next].r, curLerpWithPower);
								matSlice1[curInd1].g=mixf(matSlice0[curInd0].g, matSlice0[curInd0Next].g, curLerpWithPower);
								matSlice1[curInd1].b=mixf(matSlice0[curInd0].b, matSlice0[curInd0Next].b, curLerpWithPower);
							}
							else if(m==(i-1))
							{
								curLerpNoPower=curLerp*0.5f+0.5f;
								curPower=mixf(matSlice0[curInd0Prev].power, matSlice0[curInd0].power, curLerpNoPower);
								curLerpWithPower=pow(curLerpNoPower, curPower*8.0f);

								matSlice1[curInd1].r=mixf(matSlice0[curInd0Prev].r, matSlice0[curInd0].r, curLerpWithPower);
								matSlice1[curInd1].g=mixf(matSlice0[curInd0Prev].g, matSlice0[curInd0].g, curLerpWithPower);
								matSlice1[curInd1].b=mixf(matSlice0[curInd0Prev].b, matSlice0[curInd0].b, curLerpWithPower);
							}
							else
							{


								if(curLerp<0.5f)
								{
									curLerpNoPower=curLerp+0.5f;
									curPower=mixf(matSlice0[curInd0Prev].power, matSlice0[curInd0].power, curLerpNoPower);
									curLerpWithPower=pow(curLerpNoPower, curPower*8.0f);

									matSlice1[curInd1].r=mixf(matSlice0[curInd0Prev].r, matSlice0[curInd0].r, curLerpWithPower);
									matSlice1[curInd1].g=mixf(matSlice0[curInd0Prev].g, matSlice0[curInd0].g, curLerpWithPower);
									matSlice1[curInd1].b=mixf(matSlice0[curInd0Prev].b, matSlice0[curInd0].b, curLerpWithPower);
								}
								else
								{
									curLerpNoPower=curLerp-0.5f;
									curPower=mixf(matSlice0[curInd0].power, matSlice0[curInd0Next].power, curLerpNoPower);
									curLerpWithPower=pow(curLerpNoPower, curPower*8.0f);

									matSlice1[curInd1].r=mixf(matSlice0[curInd0].r, matSlice0[curInd0Next].r, curLerpWithPower);
									matSlice1[curInd1].g=mixf(matSlice0[curInd0].g, matSlice0[curInd0Next].g, curLerpWithPower);
									matSlice1[curInd1].b=mixf(matSlice0[curInd0].b, matSlice0[curInd0Next].b, curLerpWithPower);

								}



							}



							totN++;
						}


					}
				}





				j++;

			}





			////////////////////





			for(p=0; p<mvx; p++)
			{

				totN=0;
				for(m=0; m<j; m++)
				{
					curInd0=p+m*mvx;

					if(j==1)
					{
						curInd0Prev=curInd0;
						curInd0Next=curInd0;
					}
					else
					{
						curInd0Prev=p+(m-1)*mvx;
						curInd0Next=p+(m+1)*mvx;
					}



					if(j==0)
					{
						curRatio=(float(mvy));
					}
					else
					{
						curRatio=(float(mvy))/(float(j));
					}


					for(n=0; (n<(curRatio+0.1f))&&(totN<mvy); n++)
					{
						curLerp=((float)n)/curRatio;
						curInd1=p+totN*mvx;

						if(m==0)
						{

							curLerpWithPower=curLerp*0.5f;

							matSlice2[curInd1].r=mixf(matSlice1[curInd0].r, matSlice1[curInd0Next].r, curLerpWithPower);
							matSlice2[curInd1].g=mixf(matSlice1[curInd0].g, matSlice1[curInd0Next].g, curLerpWithPower);
							matSlice2[curInd1].b=mixf(matSlice1[curInd0].b, matSlice1[curInd0Next].b, curLerpWithPower);
						}
						else if(m==(j-1))
						{
							curLerpWithPower=curLerp*0.5f+0.5f;

							matSlice2[curInd1].r=mixf(matSlice1[curInd0Prev].r, matSlice1[curInd0].r, curLerpWithPower);
							matSlice2[curInd1].g=mixf(matSlice1[curInd0Prev].g, matSlice1[curInd0].g, curLerpWithPower);
							matSlice2[curInd1].b=mixf(matSlice1[curInd0Prev].b, matSlice1[curInd0].b, curLerpWithPower);
						}
						else
						{


							if(curLerp<0.5f)
							{
								curLerpWithPower=curLerp+0.5f;

								matSlice2[curInd1].r=mixf(matSlice1[curInd0Prev].r, matSlice1[curInd0].r, curLerpWithPower);
								matSlice2[curInd1].g=mixf(matSlice1[curInd0Prev].g, matSlice1[curInd0].g, curLerpWithPower);
								matSlice2[curInd1].b=mixf(matSlice1[curInd0Prev].b, matSlice1[curInd0].b, curLerpWithPower);
							}
							else
							{
								curLerpWithPower=curLerp-0.5f;

								matSlice2[curInd1].r=mixf(matSlice1[curInd0].r, matSlice1[curInd0Next].r, curLerpWithPower);
								matSlice2[curInd1].g=mixf(matSlice1[curInd0].g, matSlice1[curInd0Next].g, curLerpWithPower);
								matSlice2[curInd1].b=mixf(matSlice1[curInd0].b, matSlice1[curInd0Next].b, curLerpWithPower);

							}



						}



						mvInd=curInd1+k*mvx*mvy;
						rv=matSlice2[curInd1].r*255.0f;
						gv=matSlice2[curInd1].g*255.0f;
						bv=matSlice2[curInd1].b*255.0f;
						matVol[mvInd]=(0<<24)|(bv<<16)|(gv<<8)|(rv);




						totN++;
					}


				}
			}




			////////////////////










			k++;
		}



		glBindTexture(GL_TEXTURE_3D, 0);
		glBindTexture(GL_TEXTURE_3D, volIdMat);
		glTexSubImage3D(
			GL_TEXTURE_3D,
			0,

			0,
			0,
			0,

			mvx,
			mvy,
			mvz,

			GL_RGBA,
			GL_UNSIGNED_BYTE,

			matVol
		);
		glBindTexture(GL_TEXTURE_3D, 0);



	}



	matVolLock=false;

}
