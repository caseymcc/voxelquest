#ifndef _voxelquest_gameent_h_
#define _voxelquest_gameent_h_


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

#endif//_voxelquest__h_