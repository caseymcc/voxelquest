#ifndef _voxelquest_gameorg_h_
#define _voxelquest_gameorg_h_

class GameOrg
{

public:
    float gv(float* vals);




    GameOrg();


    void init(
        Singleton* _singleton,
        int _ownerUID,
        int _entType,
        int _subType
    );






    void jsonToNode(JSONValue** parentObj, GameOrgNode* curNode, bool notThePose);


    void setBinding(int actorId, bool val);

    int getPoseUID();

    void loadOrgFromFile(std::string fileName, bool notThePose);


    void saveOrgToFile(std::string fileName);

    BaseObj* getOwner();

    void setTPG(int _targetPoseGroup, int _targetPoseRLBN);

    void setToPose(
        GameOrg* otherOrg,
        float lerpAmount,
        int boneId=-1
    );

    void updatePose(double curTimeStep);

    void nodeToJSON(JSONValue** parentObj, GameOrgNode* curNode);


    void updateHandleOffset();



    void initWeapon();



    void initHuman();

private:
    const static float baseMat;
    Singleton* singleton;
    GameOrgNode* baseNode;

    GameOrgNode* allNodes[E_BONE_C_END];

    float wepLengths[E_BONE_C_END];

    FIVector4 basePosition;

    JSONValue *rootObj;


    PoseKey basePose;
    PoseKey targetPose;




    int ownerUID;
    int entType;
    int subType;
    int stepCount;



    double totTime;
    float defVecLength;





};

#endif//_voxelquest__h_