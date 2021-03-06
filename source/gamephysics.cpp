#include "voxelquest/gamephysics.h"
#include "voxelquest/myglhelper.h"
#include "voxelquest/settings.h"
#include "voxelquest/gamestate.h"
#include "voxelquest/gameentmanager.h"
#include "voxelquest/bullethelpers.h"
#include "voxelquest/gamephysrig.h"
#include "voxelquest/gameorg.h"
#include "voxelquest/gameworld.h"
#include "voxelquest/gameaudio.h"
#include "voxelquest/renderer.h"

#include <iostream>

struct CustFilterCallback: public btOverlapFilterCallback
{

    // CustFilterCallback()
    // {
    // }
    // virtual ~CustFilterCallback()
    // {
    // }

    // return true when pairs need collision
    virtual bool needBroadphaseCollision(
        btBroadphaseProxy* proxy0,
        btBroadphaseProxy* proxy1) const
    {

        if(GameState::EDIT_POSE)
        {
            return false;
        }

        bool collides=
            (
            ((proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask)!=0)&&
                ((proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask)!=0)
                );



        btCollisionObject* colObj0=(btCollisionObject*)(proxy0->m_clientObject);
        btCollisionObject* colObj1=(btCollisionObject*)(proxy1->m_clientObject);



        // if (colObj0->bodyUID != colObj1->bodyUID) {
        // 	cout << colObj0->bodyUID << " " << colObj1->bodyUID << "\n";
        // }

        collides=collides/*&&(
            (colObj0->bodyUID!=colObj1->bodyUID)&&
            (colObj0->bodyUID>-2)&&
            (colObj1->bodyUID>-2)&&
            (colObj0->heldByUID!=colObj1->bodyUID)&&
            (colObj1->heldByUID!=colObj0->bodyUID)
            )*/;



        return collides;
    }
};

GamePhysics::GamePhysics()
{

    lastBodyPick=NULL;
    lastBodyUID=-1;

}

GamePhysics::~GamePhysics()
{
    example->exitPhysics();
    delete example;
}


void GamePhysics::init(Singleton* _singleton)
{
    orig=btVector3(0.0f, 0.0f, 0.0f);
    xyMask=btVector3(1.0f, 1.0f, 0.0f);
    zMask=btVector3(0.0f, 0.0f, 1.0f);

    CONTACT_THRESH=0.2f;
    //totTime = 0.0;
    BASE_ENT_HEIGHT=2.0f*ORG_SCALE_BASE;
    BASE_ENT_RAD=1.0f*ORG_SCALE_BASE;

    std::cout<<"GamePhysics:init()\n";

    singleton=_singleton;
    myOGLApp=new MyOGLApp("yo", 640, 480);
    guiHelper=new MyGLHelper(singleton, myOGLApp);
    example=new BenchmarkDemo(guiHelper, 5);
    example->initPhysics();


    example->getWorld()->getPairCache()->setOverlapFilterCallback(new CustFilterCallback());


}


void GamePhysics::pickBody(FIVector4* mmPD)
{

    if(!(g_settings.settings[E_BS_EDIT_POSE]))
    {
        lastBodyPick=NULL;
        lastBodyUID=-1;
        return;
    }

    int bodyUID=(int)mmPD->getFW();
    int limbUID=(int)mmPD->getFZ();
    BaseObj* ge;

    if(
        (bodyUID>0)&&
        (limbUID>-1)
        )
    {
        ge=&(GameState::gem->gameObjects[bodyUID]);
        lastBodyPick=ge->bodies[limbUID].body;
        lastBodyUID=bodyUID;
    }
    else
    {
        lastBodyPick=NULL;
        lastBodyUID=-1;
    }
}


void GamePhysics::collectDebris()
{
    int i;
    FIVector4 tempVec;
    int entNum;

    for(i=0; i<debrisStack.size(); i++)
    {

        tempVec=convertToVQV(debrisStack[i].pos);
        entNum=GameState::gem->placeNewEnt(false, E_ENTTYPE_DEBRIS, &tempVec);

        //addDebris(debrisStack[i].pos);
    }
    debrisStack.clear();
}

void GamePhysics::beginDrop()
{

    std::cout<<"GamePhysics:beginDrop()\n";

    example->beginDrop(
        Renderer::cameraGetPosNoShake()->getFX(),
        Renderer::cameraGetPosNoShake()->getFY(),
        Renderer::cameraGetPosNoShake()->getFZ()
    );
}



void GamePhysics::remBoxFromObj(BaseObjType _uid)
{

    BaseObj* ge=&(GameState::gem->gameObjects[_uid]);

    int bodInd;

    bool hasRig=(
        (ge->orgId>-1)&&
        (ge->actorId>-1)
        );

    GamePhysRig* curPhysRig;

//    int i;

    GameState::gem->makeDropAll(ge->uid);



    if(hasRig)
    {
        curPhysRig=GameState::gem->gamePhysRigs[ge->actorId];
        curPhysRig->removeAllBodies();

        while(ge->bodies.size()>E_BDG_LENGTH)
        {
            ge->bodies.pop_back();
        }

    }

    for(bodInd=0; bodInd<ge->bodies.size(); bodInd++)
    { // E_BDG_LENGTH
        example->removeRigidBody(ge->bodies[bodInd].body);
    }





    // if (hasRig) {
    // 	//curPhysRig = GameState::gem->gamePhysRigs[ge->actorId];
    // 	delete GameState::gem->gamePhysRigs[ge->actorId];
    // 	GameState::gem->gamePhysRigs[ge->actorId] = NULL;

    // 	delete GameState::gem->gameOrgs[ge->orgId];
    // 	GameState::gem->gameOrgs[ge->orgId] = NULL;
    // }

    // ge->actorId = -1;
    // ge->orgId = -1;

    ge->bodies.clear();
    //ge->blockers.clear();
}

void GamePhysics::addBoxFromObj(BaseObjType _uid, bool refreshLimbs)
{



    //cout << "\n\nADD BOX\n\n";

    int i;
//    int j;
    int bodInd;
    GameOrg* curOrg=NULL;

    if(_uid<0)
    {
        return;
    }

    BaseObj* ge=&(GameState::gem->gameObjects[_uid]);

    if(ge->isHidden)
    {
        return;
    }

    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(ge->startPoint);

    btTransform trans2;
    trans2.setIdentity();
    trans2.setOrigin(ge->startPoint);

    GamePhysRig* curPhysRig;

    float objRad=0.5f;
    bool isOrg=false;

    int bodyOffset=0;

    switch(ge->entType)
    {
    case E_ENTTYPE_NPC:
    case E_ENTTYPE_WEAPON:
    {

        isOrg=true;


        if(ge->entType==E_ENTTYPE_WEAPON)
        {

        }
        else
        {


            ////////////////////////////

            if(refreshLimbs)
            {
                // todo: add stuff
            }
            else
            {
                for(i=0; i<E_BDG_LENGTH; i++)
                {
                    //btCapsuleShapeZ* capsuleShapeZ = new btCapsuleShapeZ(1.0f,BASE_ENT_HEIGHT);

                    switch(i)
                    {
                    case E_BDG_CENTER:
                        trans.setIdentity();
                        trans.setOrigin(ge->startPoint);
                        break;
                        // case E_BDG_LFOOT:
                        // 	trans.setIdentity();
                        // 	trans.setOrigin(ge->startPoint + btVector3(-1.0f, 0.0f, -1.0f));
                        // break;
                        // case E_BDG_RFOOT:
                        // 	trans.setIdentity();
                        // 	trans.setOrigin(ge->startPoint + btVector3( 1.0f, 0.0f, -1.0f));
                        // break;
                    }

                    ge->bodies.push_back(BodyStruct());

                    if(i==E_BDG_CENTER)
                    {
                        ge->bodies[i].body=example->createRigidBodyMask(
                            MASS_PER_LIMB, // 0.1
                            trans,
                            new btCapsuleShapeZ(BASE_ENT_RAD, BASE_ENT_HEIGHT),//capsuleShapeZ, //btSphereShape(BASE_ENT_HEIGHT),//
                            COL_MARKER,
                            markerCollidesWith
                        );

                        // for (j = 0; j < E_BLOCKER_LENGTH; j++) {

                        // 	switch(j) {
                        // 		case E_BLOCKER_GRAV:
                        // 			trans2.setOrigin(ge->startPoint + btVector3(0.0f,0.0f,-5.0f));
                        // 		break;
                        // 	}

                        // 	ge->blockers.push_back(BodyStruct());
                        // 	ge->blockers[j].body = example->createRigidBodyMask(
                        // 		9999.0, //mass
                        // 		trans2,
                        // 		new btSphereShape(BLOCKER_RADIUS),
                        // 		COL_BLOCKER,
                        // 		blockerCollidesWith
                        // 	);

                        // 	ge->blockers[j].body->bodyUID = _uid;
                        // 	ge->blockers[j].body->limbUID = -7;
                        // 	ge->blockers[j].body->setDamping(0.0f,0.0f);
                        // 	ge->blockers[j].body->setContactProcessingThreshold(CONTACT_THRESH);
                        // 	ge->blockers[j].isVisible = true;
                        // 	ge->blockers[j].body->setGravity(btVector3(0.0f,0.0f,0.0f));
                        // 	ge->blockers[j].mass = 0.0;
                        // 	ge->blockers[j].hasContact = false;
                        // 	ge->blockers[j].isInside = false;
                        // 	ge->blockers[j].isFalling = false;
                        // 	ge->blockers[j].inWater = false;
                        // 	ge->blockers[j].lastVel = orig;
                        // 	ge->blockers[j].totAV = orig;
                        // 	ge->blockers[j].totLV = orig;



                        // }

                    }
                    else
                    {
                        // ge->bodies[i].body = example->createRigidBodyMask(
                        // 	MASS_PER_LIMB, // 0.1
                        // 	trans,
                        // 	new btSphereShape(0.25f),//new btCapsuleShapeZ(1.0f,BASE_ENT_HEIGHT),//capsuleShapeZ,
                        // 	COL_MARKER,
                        // 	markerCollidesWith
                        // );
                    }


                    ge->bodies[i].body->setAngularFactor(orig);
                    ge->bodies[i].boneId=-1;
                    ge->bodies[i].jointType=E_JT_CONT;
                }

                bodyOffset=E_BDG_LENGTH;

            }




            ////////////////////////////


        }




        if(refreshLimbs)
        {

            delete GameState::gem->gamePhysRigs[ge->actorId];

            GameState::gem->gamePhysRigs[ge->actorId]=new GamePhysRig(
                singleton,
                ge->uid,
                example->getWorld(),
                ge->getCenterPoint(E_BDG_CENTER)
            );

        }
        else
        {
            GameState::gem->gameOrgs.push_back(new GameOrg());
            GameState::gem->gameOrgs.back()->init(singleton, ge->uid, ge->entType, ge->subType);
            ge->orgId=(int)GameState::gem->gameOrgs.size()-1;

            GameState::gem->loadDefaultPose(ge->uid);

            GameState::gem->gamePhysRigs.push_back(new GamePhysRig(
                singleton,
                ge->uid,
                example->getWorld(),
                ge->startPoint
            ));
            ge->actorId=(int)GameState::gem->gamePhysRigs.size()-1;
        }


        curPhysRig=(GameState::gem->gamePhysRigs[ge->actorId]);

        if(refreshLimbs)
        {
            curPhysRig->reinit();
        }


        for(i=0; i<curPhysRig->rigJoints.size(); i++)
        {

            if(!refreshLimbs)
            {
                ge->bodies.push_back(BodyStruct());
            }

            bodInd=i+bodyOffset;

            ge->bodies[bodInd].body=curPhysRig->rigJoints[i].body;
            ge->bodies[bodInd].boneId=curPhysRig->rigJoints[i].boneId;
            ge->bodies[bodInd].jointType=curPhysRig->rigJoints[i].jointType;
            ge->bodies[bodInd].rad=curPhysRig->rigJoints[i].rad;
            ge->bodies[bodInd].length=curPhysRig->rigJoints[i].length;
            ge->bodies[bodInd].actorJointId=i;

            // if (i == 0) {
            // 	//ge->body = curPhysRig->rigJoints[i].body;
            // 	//ge->body->setLinearFactor(orig);
            // 	ge->bodies.back().body->setAngularFactor(orig);
            // }
            // else {

            // }
        }
    }
    break;
    default:
    {

        if(refreshLimbs)
        {
            std::cout<<"Error, should not hit this point (refreshLimbs)\n";
            return;
        }

        btBoxShape* boxShape=new btBoxShape(btVector3(objRad, objRad, objRad));
        ge->bodies.push_back(BodyStruct());
        ge->bodies.back().body=example->createRigidBodyMask(
            MASS_PER_LIMB,
            trans,
            boxShape
            , COL_DYN,
            dynCollidesWith
        );
        ge->bodies.back().boneId=-1;
        ge->bodies.back().jointType=E_JT_OBJ;

        if(ge->entType==E_ENTTYPE_DEBRIS)
        {

            ge->bodies.back().body->setAngularVelocity(btVector3(
                (fGenRand2()*2.0f-1.0f),
                (fGenRand2()*2.0f-1.0f),
                (fGenRand2()*2.0f-1.0f)
            )*4.0f);
        }


    }
    break;
    }



    for(bodInd=0; bodInd<ge->bodies.size(); bodInd++)
    {
//        ge->bodies[bodInd].body->bodyUID=_uid;
//        ge->bodies[bodInd].body->limbUID=bodInd;

        ge->bodies[bodInd].body->setDamping(0.0f, 0.9f);

        ge->bodies[bodInd].body->setContactProcessingThreshold(CONTACT_THRESH);

        ge->bodies[bodInd].isVisible=true;

        if(isOrg)
        {
            curOrg=GameState::gem->gameOrgs[ge->orgId];
            ge->bodies[bodInd].isVisible=false;//(bodInd == 0);// false;//(bodInd < E_BDG_LENGTH);//

        }


        ge->bodies[bodInd].body->setGravity(
            btVector3(
            0.0f,
            0.0f,
            getConst(E_CONST_DEFAULT_GRAVITY)
        )
        );
        ge->bodies[bodInd].mass=MASS_PER_LIMB;
        ge->bodies[bodInd].hasContact=false;
        ge->bodies[bodInd].isInside=false;
        ge->bodies[bodInd].isFalling=true;
        ge->bodies[bodInd].inWater=false;
        ge->bodies[bodInd].lastVel=orig;
        ge->bodies[bodInd].totAV=orig;
        ge->bodies[bodInd].totLV=orig;


    }




}





// void motorPreTickCallback(btScalar timeStep) {

// 	// int i;

// 	// for (i = 0; i < GameState::gem->gamePhysRigs.size(); i++) {
// 	// 	GameState::gem->gamePhysRigs[i]->stepSim(timeStep);
// 	// }



// }


void GamePhysics::flushImpulses()
{

    int k;
    BaseObj* ge;

    for(k=0; k<GameState::gem->visObjects.size(); k++)
    {
        ge=&(GameState::gem->gameObjects[GameState::gem->visObjects[k]]);

        if(
            (ge->isHidden)
            )
        {

        }
        else
        {
            ge->flushImpulses();
        }
    }
}

void GamePhysics::explodeBullet(BaseObj * ge)
{
    FIVector4 newPos;

    newPos=convertToVQV(ge->getCenterPoint(E_BDG_CENTER));
    newPos.addXYZ(0.0f, 0.0f, -2.0f);

    if(g_settings.settings[E_BS_WATER_BULLET])
    {
        GameAudio::playSoundEnt(
            "bubble0",
            ge,
            0.2f
        );
    }
    else
    {
        GameAudio::playSoundEnt(
            "explosion0",
            ge,
            0.2f,
            4.0
        );

        Renderer::performCamShake(ge, 1.0f);
    }

    sphereStack.push_back(SphereStruct());
    sphereStack.back().position=convertToVQV(ge->getCenterPoint(E_BDG_CENTER));
    sphereStack.back().curRad=1.0f;
    sphereStack.back().maxRad=explodeRad;
    sphereStack.back().radVel=40.0f;
    sphereStack.back().radAcc=-5.0f;


    //gameFluid[E_FID_SML]->pushExplodeBullet(true,&newPos,boolToInt(settings[E_BS_WATER_BULLET]));
    //gameFluid[E_FID_BIG]->pushExplodeBullet(true,&newPos,boolToInt(settings[E_BS_WATER_BULLET]),explodeRad);

    explodeStack.push_back(ExplodeStruct());

    if(ge->hasBodies())
    {
        explodeStack.back().pos=ge->bodies[E_BDG_CENTER].body->getCenterOfMassPosition();
        explodeStack.back().radius=20.0f;
        explodeStack.back().power=200.0f;
    }


    GameState::gem->removeVisObject(ge->uid, true);

}

void GamePhysics::procCol(
    BaseObj** geArr,
    BodyStruct** curBodyArr
)
{

    int k;
    int otherK;
    BaseObj* grabber;
    int otherUID;
    bool doProc;
    int curBone;

    for(k=0; k<2; k++)
    {
        otherK=1-k;

        if(geArr[k]==NULL)
        {

        }
        else
        {

            curBone=getCorrectedName(curBodyArr[k]->boneId);

            if(geArr[otherK]==NULL)
            {
                // hit static obj
                //GameAudio::playSoundEnt("metalhit5",geArr[k],0.2,0.5f);
                otherUID=-1;
            }
            else
            {
                otherUID=geArr[otherK]->uid;

                if(
                    (geArr[k]->entType==E_ENTTYPE_NPC)&&
                    (geArr[otherK]->entType==E_ENTTYPE_NPC)
                    )
                {

                    if(curBodyArr[k]==NULL)
                    {

                    }
                    else
                    {



                        doProc=false;

                        switch(curBone)
                        {
                        case E_BONE_L_METACARPALS:
                        case E_BONE_L_TALUS:
                            if(geArr[k]->getActionState(E_ACT_ISSWINGING, RLBN_LEFT))
                            {
                                if(curBone==E_BONE_L_METACARPALS)
                                {
                                    if(GameState::gem->isPunching(geArr[k]->uid, RLBN_LEFT))
                                    {
                                        doProc=true;
                                    }
                                }
                                else
                                {
                                    if(GameState::gem->isKicking(geArr[k]->uid, RLBN_LEFT))
                                    {
                                        doProc=true;
                                    }
                                }
                            }
                            break;
                        case E_BONE_R_TALUS:
                        case E_BONE_R_METACARPALS:
                            if(geArr[k]->getActionState(E_ACT_ISSWINGING, RLBN_RIGT))
                            {
                                if(curBone==E_BONE_R_METACARPALS)
                                {
                                    if(GameState::gem->isPunching(geArr[k]->uid, RLBN_RIGT))
                                    {
                                        doProc=true;
                                    }
                                }
                                else
                                {
                                    if(GameState::gem->isKicking(geArr[k]->uid, RLBN_RIGT))
                                    {
                                        doProc=true;
                                    }
                                }
                            }
                            break;
                        }
                    }
                    if(doProc)
                    {
                        GameState::gem->makeHit(false, geArr[k]->uid, geArr[otherK]->uid, -1);
                    }
                }
            }

            if(geArr[k]->entType==E_ENTTYPE_WEAPON)
            {





                if(
                    (geArr[k]->isGrabbedById>-1)&&
                    (geArr[k]->isGrabbedById!=otherUID)
                    )
                {
                    grabber=&(GameState::gem->gameObjects[geArr[k]->isGrabbedById]);

                    doProc=false;
                    if(grabber->getActionState(E_ACT_ISSWINGING, RLBN_LEFT))
                    {
                        if(GameState::gem->isSwingingWeapon(grabber->uid, RLBN_LEFT))
                        {
                            doProc=true;
                        }
                    }
                    if(grabber->getActionState(E_ACT_ISSWINGING, RLBN_RIGT))
                    {
                        if(GameState::gem->isSwingingWeapon(grabber->uid, RLBN_RIGT))
                        {
                            doProc=true;
                        }
                    }

                    switch(curBone)
                    {
                    case E_BONE_WEAPON_BLADER:
                    case E_BONE_WEAPON_BLADEL:
                    case E_BONE_WEAPON_BLADEU:

                        break;
                    default:
                        doProc=false;
                        break;
                    }


                    if(doProc)
                    {
                        if(geArr[otherK]==NULL)
                        {
                            GameState::gem->makeHit(false, grabber->uid, -1, geArr[k]->uid);
                        }
                        else
                        {
                            GameState::gem->makeHit(false, grabber->uid, geArr[otherK]->uid, geArr[k]->uid);
                        }
                    }
                }
            }
        }
    }
}


void GamePhysics::collideWithWorld(double curStepTime)
{


    int i;
    int j;
    int k;
    int m;
    int p;
//    int q;




    int bodInd;
    float totForce;
    float bindingPower;
    float cellVal[4];
    btVector3 newVel;
    btVector3 segPos[2];
    btVector3 norVal;
    btVector3 norVal2;
    int segCount;
    bool lastInside;
    BodyStruct* curBody;

    BaseObj* ge;

    BaseObj* geArr[2];
    BodyStruct* curBodyArr[2];

    GameOrg* grabberOrg=NULL;
    BaseObj* grabber;


//    FIVector4* curCenterPoint;
    btDiscreteDynamicsWorld* world=example->getWorld();

    btVector3 tempBTV;
    btVector3 tempBTV2;
    btVector3 nv0;
    btVector3 nv1;


    bool doProc=false;
    bool hasContact=false;

//    int entNum;

    FIVector4 tempVec;


    float ho=getConst(E_CONST_HALF_OFFSET);
    btVector3 halfOffset=btVector3(ho, ho, ho); // <-- lol


//    float curDis;
//    float totMass;
    btVector3 dirForce;
    GameOrg* curOrg=NULL;
    GameOrgNode* curOrgNode=NULL;
    GamePhysRig* curPhysRig=NULL;
    btVector3 basePos;
    btVector3 targPos;



    collectDebris();




    for(k=0; k<GameState::gem->visObjects.size(); k++)
    {
        ge=&(GameState::gem->gameObjects[GameState::gem->visObjects[k]]);

        if(
            (ge->isHidden)
            )
        {

        }
        else
        {

            for(bodInd=0; bodInd<ge->bodies.size(); bodInd++)
            {
                ge->bodies[bodInd].hasContact=false;
                ge->bodies[bodInd].isFalling=true;
            }

        }
    }


    const btCollisionObject* bds[2];

    int numManifolds=world->getDispatcher()->getNumManifolds();
    for(i=0; i<numManifolds; i++)
    {
        btPersistentManifold* contactManifold=world->getDispatcher()->getManifoldByIndexInternal(i);



        const btCollisionObject* obA=(contactManifold->getBody0());
        const btCollisionObject* obB=(contactManifold->getBody1());

        bds[0]=obA;
        bds[1]=obB;

        hasContact=false;

        int numContacts=contactManifold->getNumContacts();
        for(j=0; j<numContacts; j++)
        {
            btManifoldPoint& pt=contactManifold->getContactPoint(j);
            if(pt.getDistance()<=CONTACT_THRESH)
            {

                hasContact=true;

                // const btVector3& ptA = pt.getPositionWorldOnA();
                // const btVector3& ptB = pt.getPositionWorldOnB();
                // const btVector3& normalOnB = pt.m_normalWorldOnB;
            }
        }


        geArr[0]=NULL;
        geArr[1]=NULL;
        curBodyArr[0]=NULL;
        curBodyArr[1]=NULL;

//        if(bds[0]->bodyUID==bds[1]->bodyUID)
//        {
//            // don't register contacts within same entity
//        }
//        else
        {
//            for(k=0; k<2; k++)
//            {
//                if(
//                    (bds[k]->bodyUID>-1)&&
//                    (bds[k]->limbUID>-1)
//                    )
//                {
//                    ge=&(GameState::gem->gameObjects[bds[k]->bodyUID]);
//                    curBody=&(ge->bodies[bds[k]->limbUID]);
//
//
//
//                    if(
//                        (ge->isHidden)
//                        )
//                    {
//
//                    }
//                    else
//                    {
//
//                        if(hasContact)
//                        {
//                            geArr[k]=ge;
//                            curBodyArr[k]=curBody;
//
//                            curBody->hasContact=true;
//                            curBody->isFalling=false;
//                        }
//
//                    }
//                }
//            }

            // if (
            // 	(geArr[0] == NULL) ||
            // 	(geArr[1] == NULL) ||	
            // 	(curBodyArr[0] == NULL) ||
            // 	(curBodyArr[1] == NULL)
            // ) {

            // }
            // else {


            procCol(geArr, curBodyArr);

            //}

        }
    }



    btVector3 difVec=orig;
    //btVector3 totVec = orig;
    bool hasRig=false;
    bool animatedRig=false;
    doProc=false;
    float angDamp=getConst(E_CONST_ANGDAMP);



    if(VOXEL_COLLISION)
    {
        for(k=0; k<GameState::gem->visObjects.size(); k++)
        {
            ge=&(GameState::gem->gameObjects[GameState::gem->visObjects[k]]);

            if(
                (ge->isHidden)
                )
            {

            }
            else
            {
                for(bodInd=0; bodInd<1; bodInd++)
                { //ge->bodies.size()
                    curBody=&(ge->bodies[bodInd]);

                    switch(curBody->jointType)
                    {
                    case E_JT_LIMB:
                    case E_JT_BALL:
                    case E_JT_OBJ:

                        segCount=0;

                        // segCount = 1;
                        // segPos[0] = curBody->body->getCenterOfMassPosition() + halfOffset -
                        // 	btVector3(0.0f,0.0f,getConst(E_CONST_COLDEPTH_LIMB));
                        break;
                        break;
                    case E_JT_NORM:
                        segCount=0;
                        break;
                    case E_JT_CONT:

                        if(g_settings.settings[E_BS_TURN_BASED])
                        {
                            segCount=0;

                            curBody->hasContact=true;
                            curBody->isFalling=!(curBody->hasContact);
                            basePos=(ge->tbPos+btVector3(0.5f, 0.5f, 0.5f+getConst(E_CONST_TBSNAP_ZOFFSET)));
                            difVec=basePos-curBody->body->getCenterOfMassPosition();


                            // move limbs towards pose


                            ge->setLinVel(
                                difVec*getConst(E_CONST_TBSNAP_MULT),
                                bodInd
                            );

                            if(ge->getPlanarVel()>0.1)
                            {
                                ge->setActionState(E_ACT_ISWALKING, RLBN_NEIT, true);
                            }


                        }
                        else
                        {

                            // ge->setBlockerPosXY(
                            // 	E_BLOCKER_GRAV,
                            // 	curBody->body->getCenterOfMassPosition()
                            // );

                            segCount=2;
                            segPos[0]=curBody->body->getCenterOfMassPosition()+halfOffset-
                                btVector3(0.0f, 0.0f, getConst(E_CONST_COLDEPTH_CONT));

                            newVel=curBody->body->getLinearVelocity();
                            newVel*=xyMask;
                            if(newVel.fuzzyZero())
                            {
                                //segCount = 1;
                            }
                            else
                            {
                                newVel.normalize();

                            }


                            newVel*=getConst(E_CONST_COLDEPTH_WALL_XY);
                            newVel+=btVector3(0.0f, 0.0f, getConst(E_CONST_COLDEPTH_WALL_Z));

                            segPos[1]=
                                curBody->body->getCenterOfMassPosition()+
                                halfOffset+
                                newVel;


                            // segPos[1] = curBody->body->getCenterOfMassPosition() +
                            // 	zMask;
                        }





                        break;

                    }


                    for(p=0; p<segCount; p++)
                    {

                        norVal=GameState::gw->getNormalAtCoord(
                            segPos[p], cellVal
                        );


                        // if (!VOXEL_COLLISION) {
                        // 	if (p == 0) {
                        // 		// collision below body

                        // 		lastInside = curBody->isInside;
                        // 		curBody->isInside = (cellVal[3] > 0.5f);

                        // 		curBody->hasContact = (curBody->hasContact)||(cellVal[3] > 0.01f);
                        // 		curBody->isFalling = !(curBody->hasContact);

                        // 		if (cellVal[3] > 0.1f) {
                        // 			ge->moveOffsetBlocker(
                        // 				E_BLOCKER_GRAV,
                        // 				btVector3(0.0f,0.0f,cellVal[3]*0.1f)
                        // 			);

                        // 		}
                        // 		else {
                        // 			ge->moveOffsetBlocker(
                        // 				E_BLOCKER_GRAV,
                        // 				btVector3(0.0f,0.0f,-1.0f)
                        // 			);
                        // 			if (
                        // 				ge->blockers[E_BLOCKER_GRAV].body->getCenterOfMassPosition().distance(
                        // 					ge->getCenterPoint(E_BDG_CENTER)
                        // 				) > 5.0f
                        // 			) {
                        // 				ge->moveOffsetBlocker(
                        // 					E_BLOCKER_GRAV,
                        // 					btVector3(0.0f,0.0f,1.0f)
                        // 				);
                        // 			}
                        // 		}

                        // 	}
                        // }




                        if(p==0)
                        {
                            // collision below body

                            lastInside=curBody->isInside;
                            curBody->isInside=(cellVal[3]>0.5f);

                            curBody->hasContact=(curBody->hasContact)||(cellVal[3]>0.01f);
                            curBody->isFalling=!(curBody->hasContact);

                            if(
                                (cellVal[3]>0.01f)&&
                                (
                                //(curBody->jointType == E_JT_CONT) ||
                                (
                                (ge->entType==E_ENTTYPE_WEAPON)&&
                                (ge->isGrabbedById<0)
                                )||
                                (curBody->boneId<0)
                                )
                                )
                            {
                                ge->multVel(bodInd, btVector3(
                                    getConst(E_CONST_WALKING_FRIC),
                                    getConst(E_CONST_WALKING_FRIC),
                                    1.0f
                                ));
                            }
                            else
                            {
                                ge->multVel(bodInd, btVector3(
                                    getConst(E_CONST_AIR_RESIST),
                                    getConst(E_CONST_AIR_RESIST),
                                    1.0f
                                ));
                            }




                            if(cellVal[3]>0.01f)
                            {

                                if(!ge->isDead())
                                {
                                    ge->multVelAng(bodInd, btVector3(angDamp, angDamp, angDamp));
                                }


                                // curBody->body->setGravity(
                                // 	btVector3(
                                // 		0.0f,
                                // 		0.0f,
                                // 		(cellVal[3])*getConst(E_CONST_ANTIGRAV)
                                // 	)
                                // );

                                // curBody->body->setGravity(
                                // 	btVector3(
                                // 		0.0f,
                                // 		0.0f,
                                // 		(cellVal[3]-0.5f)*getConst(E_CONST_ANTIGRAV)
                                // 	)
                                // );

                                if(
                                    (curBody->body->getLinearVelocity().getZ()<0.0f)&&
                                    (cellVal[3]>getConst(E_CONST_VEL_STOP_THRESH))
                                    )
                                {
                                    ge->multVel(bodInd, xyMask);
                                    //ge->moveOffset(btVector3(0,0,clampfZO(cellVal[3]-0.1f)), bodInd);
                                }

                                ge->addVel(bodInd, btVector3(
                                    0.0f, 0.0f, clampfZO(
                                    cellVal[3]-getConst(E_CONST_VEL_STOP_THRESH)
                                )*getConst(E_CONST_PUSH_UP_AMOUNT)
                                ));

                                if(norVal.fuzzyZero())
                                {

                                }
                                else
                                {

                                    if(ge->getActionState(E_ACT_ISWALKING, RLBN_NEIT))
                                    {
                                        ge->addVel(bodInd,
                                            norVal*xyMask *
                                            cellVal[3]*getConst(E_CONST_NOR_PUSH)
                                        );
                                    }

                                }



                                if(
                                    (ge->entType==E_ENTTYPE_WEAPON)&&
                                    (ge->isGrabbedById>-1)&&
                                    (cellVal[3]>0.4f)
                                    )
                                {
                                    geArr[0]=ge;
                                    geArr[1]=NULL;
                                    curBodyArr[0]=curBody;
                                    curBodyArr[1]=NULL;
                                    procCol(geArr, curBodyArr);
                                }




                                // if (cellVal[3] > 0.2f) {
                                // 	ge->multVel(bodInd, btVector3(1.0f,1.0f,-1.0f));
                                // }


                                // ge->multVel(bodInd, btVector3(
                                // 	1.0f,
                                // 	1.0f,
                                // 	getConst(E_CONST_ZDAMP_GROUND)
                                // ));


                            }
                            else
                            {
                                // curBody->body->setGravity(
                                // 	btVector3(
                                // 		0.0f,
                                // 		0.0f,
                                // 		getConst(E_CONST_DEFAULT_GRAVITY)
                                // 	)
                                // );
                            }
                            curBody->body->setActivationState(ACTIVE_TAG);
                        }
                        else
                        {
                            // collision in direction of body velocity

                            if(norVal.fuzzyZero())
                            {

                            }
                            else
                            {
                                newVel=curBody->body->getLinearVelocity();
                                if(newVel.fuzzyZero())
                                {

                                }
                                else
                                {
                                    newVel.normalize();
                                    // ge->addVel(
                                    // 	bodInd,
                                    // 	btVector3(
                                    // 		newVel.getX()*norVal.getX()*cellVal[3]*getConst(E_CONST_WALL_RESIST),
                                    // 		newVel.getY()*norVal.getY()*cellVal[3]*getConst(E_CONST_WALL_RESIST),
                                    // 		0.0f // newVel.getZ()*norVal.getZ()	
                                    // 	)
                                    // );

                                    if(newVel.getZ()>0.0f)
                                    {
                                        ge->multVel(
                                            bodInd,
                                            btVector3(
                                            clampfZO(0.5f-cellVal[3]),
                                            clampfZO(0.5f-cellVal[3]),
                                            1.0f
                                        )
                                        );
                                    }



                                }
                            }


                        }







                        // if (ge->isDead()) {
                        // 	curBody->body->setGravity(btVector3(0.0f,0.0f,-5.0f));
                        // }


                        // newVel = curBody->body->getLinearVelocity();
                        // if ((newVel.fuzzyZero())) {

                        // }
                        // else {
                        // 	newVel.normalize();

                        // 	if (
                        // 		(cellVal[3] > 0.5f)
                        // 		//&& (newVel.dot(norVal) < -0.2f)	
                        // 	) {
                        // 		ge->setLinVel(norVal*10.0f,bodInd);
                        // 	}

                        // }

                        // if (norVal.getZ() > 0.0f) {
                        // 	ge->multVel(bodInd, btVector3(abs(norVal.getZ()),abs(norVal.getZ()),1.0f));

                        // 	//ge->addVel(bodInd, btVector3(0.0f,0.0f,(1.0-norVal.getZ()));
                        // }





                        // ge->applyImpulse(
                        // 	cellVal[3]*norVal*curStepTime*getConst(E_CONST_PUSH_UP_AMOUNT),
                        // 	bodInd,
                        // 	false
                        // );


                        // ge->multVel(bodInd, btVector3(
                        // 	1.0f-cellVal[0],
                        // 	1.0f-cellVal[1],
                        // 	1.0f-cellVal[2]
                        // ));

                        //ge->addVel(bodInd, norVal2*curStepTime*200.0f);

                        // curBody->body->setGravity(
                        // 	btVector3(
                        // 		0.0f,
                        // 		0.0f,
                        // 		mixf(-10.0f,10.0f,cellVal[3])
                        // 	)
                        // );
                        // if (
                        // 	(cellVal[3] > 0.5f)
                        // 	// && (curBody->body->getLinearVelocity().getZ() < 0.0)
                        // ) {
                        // 	ge->multVel(bodInd, btVector3(-1.0f,-1.0f,-1.0f));
                        // }

                        //ge->addVel(bodInd, norVal*curStepTime*20.0f);




                        // tempBTV = curBody->body->getLinearVelocity();
                        // tempBTV *= xyMask;

                        // if ((tempBTV.fuzzyZero())) {

                        // }
                        // else {
                        // 	tempBTV.normalize();
                        // }
                        // tempBTV *= 0.5f;


                        // cellVal[0] = GameState::gw->getCellAtCoordsLin(
                        // 	segPos[p] + btVector3(0.0,0.0,-0.5f)
                        // );


                        // tempBTV += btVector3(0.0f,0.0f,1.5f);


                        // cellVal[1] = GameState::gw->getCellAtCoordsLin(
                        // 	segPos[p] + tempBTV
                        // );









                        // curBody->body->setGravity(
                        // 	btVector3(
                        // 		0.0f,
                        // 		0.0f,
                        // 		mixf(-10.0f,10.0f,cellVal[0])
                        // 	)
                        // );
                        // if (
                        // 	(cellVal[0] > 0.5f) &&
                        // 	(curBody->body->getLinearVelocity().getZ() < 0.0)
                        // ) {
                        // 	ge->multVel(bodInd, xyMask);
                        // }

                        // if (cellVal[1] > 0.1) {
                        // 	ge->multVel(bodInd, btVector3(1.0f-cellVal[1],1.0f-cellVal[1],1.0f));
                        // 	ge->addVel(bodInd, xyMask);
                        // }



                        //if (lastInside != curBody->isInside) {
                            // if (curBody->isInside) {

                            // 	if (q == 0) {
                            // 		//ge->multVelocity(bodInd, xyMask);
                            // 		curBody->body->setGravity(btVector3(0.0f,0.0f,10.0f));
                            // 	}
                            // 	else {
                            // 		//ge->multVelocity(bodInd, btVector3(-0.9f,-0.9f,1.0f));
                            // 	}


                            // }
                            // else {
                            // 	curBody->body->setGravity(btVector3(0.0f,0.0f,-10.0f));
                            // }
                        //}





                    }


                }
            }
        }
    }







    for(k=0; k<GameState::gem->visObjects.size(); k++)
    {
        ge=&(GameState::gem->gameObjects[GameState::gem->visObjects[k]]);

        hasRig=false;
        animatedRig=false;

        if(
            (ge->isHidden)
            )
        {

        }
        else
        {


            hasRig=(
                (ge->orgId>-1)&&
                (ge->actorId>-1)
                );


            //totVec = orig;


            if(hasRig)
            {
                curPhysRig=GameState::gem->gamePhysRigs[ge->actorId];
                curOrg=GameState::gem->gameOrgs[ge->orgId];
                animatedRig=(ge->entType==E_ENTTYPE_NPC);
                ge->clearAABB(&(ge->aabbMinSkel), &(ge->aabbMaxSkel));

                if(animatedRig)
                {

                    // if (ge->getActionState(E_ACT_ISWALKING,RLBN_NEIT)) {
                    // 	ge->bodies[E_BDG_CENTER].body->setFriction(getConst(E_CONST_WALKING_FRIC));
                    // }
                    // else {
                    // 	ge->bodies[E_BDG_CENTER].body->setFriction(getConst(E_CONST_STANDING_FRIC));
                    // }




                    if(ge->isDead())
                    {
                        curOrg->setTPG(E_PG_DEAD, RLBN_NEIT);
                    }
                    else if(ge->getActionState(E_ACT_ISHIT, RLBN_NEIT))
                    {
                        curOrg->setTPG(E_PG_FLAIL, RLBN_NEIT);
                    }
                    else if(ge->getActionState(E_ACT_ISSWINGING, RLBN_RIGT))
                    {
                        curOrg->setTPG(ge->swingType[RLBN_RIGT], RLBN_RIGT);
                    }
                    else if(ge->getActionState(E_ACT_ISSWINGING, RLBN_LEFT))
                    {
                        curOrg->setTPG(ge->swingType[RLBN_LEFT], RLBN_LEFT);
                    }
                    else if(
                        ge->getActionState(E_ACT_ISJUMPING, RLBN_NEIT)&&
                        (ge->bodies[E_BDG_CENTER].body->getLinearVelocity().getZ()>=0.0f)
                        )
                    {
                        curOrg->setTPG(E_PG_JUMP, RLBN_NEIT);
                    }
                    else if(ge->getActionState(E_ACT_ISPICKINGUP, RLBN_NEIT))
                    {
                        curOrg->setTPG(E_PG_PICKUP, RLBN_NEIT);
                    }
                    else if(
                        ge->getActionState(E_ACT_ISWALKING, RLBN_NEIT)

                        // && (ge->getPlanarVel() > E_CONST_WALKANIM_THRESH)
                        )
                    {
                        curOrg->setTPG(E_PG_WALKFORWARD, RLBN_NEIT);
                    }
                    else
                    {
                        curOrg->setTPG(E_PG_IDLE, RLBN_NEIT);
                    }

                    if(curOrg->targetPose.group!=E_PG_JUMP)
                    {
                        ge->setActionState(E_ACT_ISJUMPING, RLBN_NEIT, false);
                    }


                    curOrg->updatePose(curStepTime);

                    ge->wakeAll();

                }


            }

            for(bodInd=0; bodInd<ge->bodies.size(); bodInd++)
            {
                curBody=&(ge->bodies[bodInd]);




                //$$$$$$$$$$$



                //////////////////////
                // APPLY FORCES
                //////////////////////

                ge->applyImpulses((float)curStepTime, bodInd);



                if(
                    hasRig
                    &&(
                    animatedRig||
                    (ge->isGrabbedById>-1)
                    )
                    )
                {

                    if(curBody->boneId>-1)
                    {
                        curOrgNode=curOrg->allNodes[curBody->boneId];
                    }
                    else
                    {
                        curOrgNode=NULL;
                    }



                    if(curOrgNode==NULL)
                    {

                        // difVec = orig;

                        // switch (bodInd) {
                        // 	case E_BDG_CENTER:

                        // 		// targPos = 
                        // 		// 	ge->bodies[E_BDG_LFOOT].body->getCenterOfMassPosition()*0.5f +
                        // 		// 	ge->bodies[E_BDG_RFOOT].body->getCenterOfMassPosition()*0.5f
                        // 		// 	//+ btVector3(0.0f, 0.0f, 1.5f)
                        // 		// ;

                        // 		// difVec = targPos - curBody->body->getCenterOfMassPosition();
                        // 		// //difVec = basePos - curBody->body->getCenterOfMassPosition();
                        // 	break;
                        // 	case E_BDG_LFOOT:
                        // 		targPos = ge->bodies[E_BDG_CENTER].body->getCenterOfMassPosition();
                        // 		curDis = ge->bodies[E_BDG_LFOOT].body->getCenterOfMassPosition().distance(
                        // 			targPos
                        // 		);
                        // 		if (curDis > 2.5f) {
                        // 			difVec = targPos - curBody->body->getCenterOfMassPosition();
                        // 		}
                        // 		if (curDis < 0.5f) {
                        // 			difVec = -(targPos - curBody->body->getCenterOfMassPosition());
                        // 		}

                        // 	break;
                        // 	case E_BDG_RFOOT:
                        // 		targPos = ge->bodies[E_BDG_CENTER].body->getCenterOfMassPosition();
                        // 		curDis = ge->bodies[E_BDG_RFOOT].body->getCenterOfMassPosition().distance(
                        // 			targPos
                        // 		);
                        // 		if (curDis > 2.5f) {
                        // 			difVec = targPos - curBody->body->getCenterOfMassPosition();
                        // 		}
                        // 		if (curDis < 0.5f) {
                        // 			difVec = -(targPos - curBody->body->getCenterOfMassPosition());
                        // 		}
                        // 	break;
                        // }

                        // ge->applyImpulse(
                        // 	difVec*curStepTime*curBody->mass*200.0f, // *MASS_PER_LIMB*2.0f*10.0f*curStepTime,
                        // 	false,
                        // 	bodInd
                        // );

                    }
                    else
                    {


                        if(ge->isGrabbedById>-1)
                        {
                            grabber=&(GameState::gem->gameObjects[ge->isGrabbedById]);
                            bindingPower=1.0f;
                        }
                        else
                        {
                            grabber=ge;
                            bindingPower=ge->bindingPower*getConst(E_CONST_BINDING_POW_MAX);
                        }



                        grabber->bodies[E_BDG_CENTER].body->getWorldTransform().getOpenGLMatrix(myMat);
                        myMatrix4=Matrix4(myMat);

                        switch(curBody->jointType)
                        {
                        case E_JT_LIMB:
                            tempBTV=convertToBTV(curOrgNode->orgTrans[1]);
                            break;
                        case E_JT_BALL:
                            tempBTV=convertToBTV(curOrgNode->orgTrans[2]);
                            break;
                        case E_JT_NORM:

                            break;
                        }



                        myVector4=Vector4(
                            tempBTV.getX(),
                            tempBTV.getY(),
                            tempBTV.getZ(),
                            1.0f
                        );
                        resVector4=myMatrix4*myVector4;
                        basePos=btVector3(resVector4.x, resVector4.y, resVector4.z);


                        ge->addAABBPoint(&(ge->aabbMinSkel), &(ge->aabbMaxSkel), basePos);

                        basePos+=grabber->skelOffset;

                        difVec=basePos-curBody->body->getCenterOfMassPosition();


                        // move limbs towards pose


                        ge->setLinVel(
                            curBody->body->getLinearVelocity()*(1.0f-bindingPower)
                            +difVec*getConst(E_CONST_BINDING_MULT)*bindingPower,
                            bodInd
                        );

                    }


                }


                if(
                    (GameState::gem->selObjInd==ge->uid)&&
                    GameState::markerFound &&
                    GameState::gem->isDraggingObject&&
                    (GameState::gem->draggingFromType==E_DT_WORLD_OBJECT)
                    )
                {

                    if(lastBodyPick==NULL)
                    {

                    }
                    else
                    {
//                        if(
//                            lastBodyPick->limbUID==
//                            curBody->body->limbUID
//                            )
//                        {
//
//                            //totMass = ge->getTotalMass();
//
//                            //ge->applyAngularImpulse(btVector3(0,0,-0.02)/STEP_TIME_IN_SEC, false, curBody->body->limbUID);
//
//
//                        }
                    }


                    // ge->applyImpulse(
                    // 	btVector3(
                    // 		( singleton->worldMarker.getFX() - curBody->body->getCenterOfMassPosition().getX() ),
                    // 		( singleton->worldMarker.getFY() - curBody->body->getCenterOfMassPosition().getY() ),
                    // 		-(curBody->body->getCenterOfMassPosition().getZ() - (8.0f + singleton->worldMarker.getFZ()))
                    // 	)*totMass*100.0f*curStepTime,
                    // 	false,
                    // 	0
                    // );





                }



                for(m=0; m<sphereStack.size(); m++)
                {
                    tempBTV=curBody->body->getCenterOfMassPosition();
                    tempBTV2=convertToBTV(sphereStack[m].position);

                    totForce=(
                        1.0f-clampfZO(
                        tempBTV.distance(tempBTV2)/(sphereStack[m].curRad*5.0f)
                        )
                        )*5.0f; // * singleton->sphereStack[m].power;
                    dirForce=tempBTV-tempBTV2;
                    dirForce.normalize();
                    dirForce=dirForce*totForce;

                    dirForce.setZ(totForce);

                    if(dirForce.length()>2.0f)
                    {
                        ge->hitCooldown=std::max(ge->hitCooldown, 2);
                    }

                    ge->applyImpulse(dirForce*(float)curStepTime*5.0f*curBody->mass, false, bodInd);
                }


                //////////////////////
                // END APPLY FORCES
                //////////////////////





                nv0=curBody->body->getLinearVelocity();
                nv1=curBody->lastVel;

                if((nv0.fuzzyZero())||(nv1.fuzzyZero()))
                {

                }
                else
                {
                    nv0.normalize();
                    nv1.normalize();


                    if(
                        //(!(curBody->isInside)) &&
                        (
                        curBody->lastVel.length()>0.5f
                        )&&
                        (
                        (nv0.dot(nv1))<0.8f
                        )&&
                        (bodInd==0)&&
                        (ge->isGrabbedById<0)

                        )
                    {


                        GameState::gw->fireEvent(
                            ge->uid,
                            EV_COLLISION,
                            clampfZO((curBody->lastVel.length()-0.5f)/16.0f)*
                            (1.0f-clampfZO(ge->getCenterPointFIV(bodInd)->distance(Renderer::cameraGetPosNoShake())/(50.0f)))
                        );
                    }
                }



                curBody->lastVel=curBody->body->getLinearVelocity();



                if(ge->entType==E_ENTTYPE_BULLET)
                {
                    if(
                        (!(curBody->isFalling))&&
                        (curBody->body->getLinearVelocity().length()<0.5)
                        )
                    {
                        explodeBullet(ge);
                    }
                }




                //$$$$$$$$$$$

            }

            ge->zeroZ=false;

            //ge->skelOffset *= 0.99;

            if(ge->baseContact()&&hasRig&&animatedRig)
            {

                ge->skelOffset=btVector3(
                    0.0f,
                    0.0f,

                    (
                    (ge->getCenterPoint(E_BDG_CENTER).getZ()-(BASE_ENT_HEIGHT+BASE_ENT_RAD)*0.5f)-
                    ge->aabbMinSkel.getZ()
                )

                );

                if(g_settings.settings[E_BS_EDIT_POSE])
                {
                    ge->skelOffset+=btVector3(
                        0.0f,
                        0.0f,
                        1.0f
                    );
                }
                else
                {

                }

            }
            else
            {
                ge->skelOffset*=0.99f;
            }

        }


    }

    explodeStack.clear();



}

void GamePhysics::remFarAway()
{
    int k;
    BaseObj* ge;

    btVector3 camPos;

    if(GameState::gem->getCurActor()==NULL)
    {
        camPos=convertToBTV(*Renderer::cameraGetPosNoShake());
    }
    else
    {
        camPos=GameState::gem->getCurActor()->getCenterPoint(E_BDG_CENTER);
    }




    for(k=0; k<GameState::gem->visObjects.size(); k++)
    {
        ge=&(GameState::gem->gameObjects[GameState::gem->visObjects[k]]);

        if(
            (ge->isHidden)
            )
        {

        }
        else
        {
            if(ge->getCenterPoint(E_BDG_CENTER).distance(camPos)>getConst(E_CONST_REM_DIS_THRESH))
            {
                GameState::gem->removeVisObject(ge->uid, GameState::gem->isRecycledFunc(ge->entType));
                k--;
            }
        }
    }
}

void GamePhysics::updateAll()
{
    int i;
    for(i=0; i<getConst(E_CONST_PHYS_STEPS_PER_FRAME); i++)
    {
        collideWithWorld(g_settings.STEP_TIME_IN_SEC);
        example->stepSimulation((float)g_settings.STEP_TIME_IN_SEC);
    }

    //while (singleton->totTimePassedPhysics > getConst(E_CONST_STEP_TIME_IN_MICRO_SEC)) {
        //totTime += STEP_TIME_IN_SEC;
        // collideWithWorld(STEP_TIME_IN_SEC);
        // example->stepSimulation(STEP_TIME_IN_SEC);
        //singleton->totTimePassedPhysics -= getConst(E_CONST_STEP_TIME_IN_MICRO_SEC);
    //}

    flushImpulses();
    remFarAway();
}

void GamePhysics::updateBullets()
{
    int i;
    SphereStruct* ss;

    for(i=0; i<sphereStack.size(); i++)
    {
        ss=&(sphereStack[i]);

        ss->radVel+=ss->radAcc*(float)GameState::timeDelta;
        ss->curRad+=ss->radVel*(float)GameState::timeDelta;

        if(ss->curRad>=ss->maxRad)
        {
            ss->curRad=ss->maxRad;
            ss->radVel=0.0f;
        }
    }

    for(i=0; i<sphereStack.size(); i++)
    {
        ss=&(sphereStack[i]);
        if(ss->curRad<=0.0)
        {
            sphereStack.erase(sphereStack.begin()+i);
        }
    }
}