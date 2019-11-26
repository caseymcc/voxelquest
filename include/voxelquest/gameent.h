#ifndef _voxelquest_gameent_h_
#define _voxelquest_gameent_h_

#include "voxelquest/gamepageholder.h"
#include "voxelquest/gameentnode.h"

enum E_ENT_TYPES
{
    E_ET_GEOM,
    //E_ET_ACTOR,
    E_ET_LIGHT,
    E_ET_LENGTH

};

class Singleton;

class GameEnt
{

public:
    GameEnt();

    float gv(float* vals);

    void init(
        Singleton* _singleton
    );



    void initHuman();

private:
    Singleton* singleton;
    GamePageHolder* gph;
    GameEntNode* baseNode;
    FIVector4 basePosition;

    float defVecLength;

};

struct EntVec
{
    std::vector<GameEnt> data;
};

#endif//_voxelquest__h_