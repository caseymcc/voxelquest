#ifndef _voxelquest_gamephysics_h_
#define _voxelquest_gamephysics_h_

class GamePhysics {
public:
    GamePhysics();
    ~GamePhysics();

    void init(Singleton* _singleton);

    void pickBody(FIVector4* mmPD);

    void collectDebris();

    void remBoxFromObj(BaseObjType _uid);

    void addBoxFromObj(BaseObjType _uid, bool refreshLimbs);

    void flushImpulses();

    void procCol(
        BaseObj** geArr,
        BodyStruct** curBodyArr
    );

    void collideWithWorld(double curStepTime);

    void remFarAway();

    void updateAll();

private:
	Singleton* singleton;
	
	BenchmarkDemo* example;
	MyOGLApp* myOGLApp;
	GUIHelperInterface* guiHelper;
	
	float myMat[16];
	float BASE_ENT_HEIGHT;
	float BASE_ENT_RAD;
	float CONTACT_THRESH;
	//double totTime;
	
	Matrix4 myMatrix4;
	Vector4 myVector4;
	Vector4 resVector4;
	
	btRigidBody* lastBodyPick;
	int lastBodyUID;
	
	
	btVector3 orig;
	btVector3 xyMask;
	btVector3 zMask;
	
    
	
	
	
	
};








#endif//_voxelquest__h_