#ifndef _voxelquest_gameplant_h_
#define _voxelquest_gameplant_h_

enum E_PLANT_TYPES
{
    E_PT_OAK_TRUNK,
    E_PT_OAK_ROOTS,
    E_PT_OAK2_TRUNK,
    E_PT_OAK2_ROOTS,
    E_PT_BARE_OAK_TRUNK,
    E_PT_BARE_OAK_ROOTS,
    E_PT_LENGTH
};

struct PlantRules
{
    float numChildren[2];
    float divergenceAngleV[2];
    float begThickness;
    float endThickness;
    float curLength[MAX_PLANT_GEN];
    float sphereGen;
    float sphereRad;
    //float baseLength;
    //float nodeLengthMultiplier;
    float numGenerations;
    float angleUniformityU;
    float isInit;


};

class GamePlant
{

public:
    GamePlant();

    static void initAllPlants(Singleton* _singleton);

    float gv(float* vals);

    void init(
        Singleton* _singleton,
        PlantRules* _rootRules,
        PlantRules* _trunkRules,
        FIVector4* _origin
    );




    void initBase(
        PlantRules* rules,
        GamePlantNode* curNode,
        FIVector4* baseVec
    );



    void applyRules(
        PlantRules* rules,
        GamePlantNode* curParent,
        int curGen,
        int maxGen,
        //float curLength,
        float totLength,
        float maxLength
    );

    static PlantRules allPlantRules[E_PT_LENGTH];
    static GamePlant* gamePlants[E_PT_LENGTH/2];
//private:

    Singleton* singleton;

    FIVector4 origin;
    FIVector4 tempv0;
    FIVector4 tempv1;
    FIVector4 tempv2;
    FIVector4 tempv3;

    FIVector4 rootVec;
    FIVector4 trunkVec;

    GamePlantNode* trunkNode;
    GamePlantNode* rootsNode;

    PlantRules* rootRules;
    PlantRules* trunkRules;

    // static void sv(float* vals, float v0, float v1) {
    // 	vals[0] = v0;
    // 	vals[1] = v1;
    // }



};

#endif//_voxelquest__h_