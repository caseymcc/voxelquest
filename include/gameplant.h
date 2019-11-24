
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

private:

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




    static PlantRules allPlantRules[E_PT_LENGTH];

    // static void sv(float* vals, float v0, float v1) {
    // 	vals[0] = v0;
    // 	vals[1] = v1;
    // }



};
