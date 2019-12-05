#include "voxelquest/baseobject.h"
#include "voxelquest/entenums.h"
#include "voxelquest/bullethelpers.h"

#include <iostream>
#include <algorithm>
#include <cassert>

bool BaseObj::hasAtLeast(int status, int val)
{
    return (statSheet.curStatus[status]>=val);
}

void BaseObj::modifyStatus(int status, int modVal)
{
    statSheet.curStatus[status]+=modVal;


    if(statSheet.curStatus[status]<0)
    {
        statSheet.curStatus[status]=0;
    }
    if(statSheet.curStatus[status]>statSheet.maxStatus[status])
    {
        statSheet.curStatus[status]=statSheet.maxStatus[status];
    }

}

btVector3 BaseObj::getUnitBounds(bool getMax)
{

    if(bodies.size()<1)
    {
        std::cout<<"ERROR: getUnitBounds() with no bodies\n";
    }

    btVector3 cp=getCenterPoint(E_BDG_CENTER);

    float diamXY=2.0f;
    float diamZ=4.0f;

    btVector3 newRad=btVector3(diamXY*0.5f, diamXY*0.5f, diamZ*0.5f);


    switch(entType)
    {
    case E_ENTTYPE_NPC:

        break;
    default:

        break;
    }

    cp=btVector3(floor(cp.getX()), floor(cp.getY()), cp.getZ());

    if(getMax)
    {
        cp+=btVector3(1.0f, 1.0f, newRad.getZ());
    }
    else
    {
        cp-=btVector3(0.0f, 0.0f, newRad.getZ());
    }

    return cp;

}




bool BaseObj::holdingWeapon(int handNum)
{

    if(handNum==-1)
    {
        return (
            (isGrabbingId[RLBN_LEFT]>-1)||
            (isGrabbingId[RLBN_RIGT]>-1)
            );
    }
    else
    {
        return (isGrabbingId[handNum]>-1);
    }


}


void BaseObj::setDamping(float linear, float angular)
{
    int i;

    for(i=0; i<bodies.size(); i++)
    {
        bodies[i].body->setDamping(linear, angular);
    }
}

void BaseObj::clearAABB(btVector3* aabbMin, btVector3* aabbMax)
{
    *aabbMin=btVector3(FLT_MAX, FLT_MAX, FLT_MAX);
    *aabbMax=btVector3(FLT_MIN, FLT_MIN, FLT_MIN);
}

void BaseObj::addAABBPoint(btVector3* aabbMin, btVector3* aabbMax, btVector3 newPoint)
{
    btVector3 tempv;

    tempv=btVector3(
        std::max(newPoint.getX(), aabbMax->getX()),
        std::max(newPoint.getY(), aabbMax->getY()),
        std::max(newPoint.getZ(), aabbMax->getZ())
    );

    *aabbMax=tempv;

    tempv=btVector3(
        std::min(newPoint.getX(), aabbMin->getX()),
        std::min(newPoint.getY(), aabbMin->getY()),
        std::min(newPoint.getZ(), aabbMin->getZ())
    );

    *aabbMin=tempv;
}


bool BaseObj::hasBodies()
{
    return (bodies.size()>0);
}

void BaseObj::multVel(int i, btVector3 velMod)
{

    bodies[i].body->setLinearVelocity(
        bodies[i].body->getLinearVelocity() * velMod
    );
}

void BaseObj::multVelAng(int i, btVector3 velMod)
{

    bodies[i].body->setAngularVelocity(
        bodies[i].body->getAngularVelocity() * velMod
    );
}

void BaseObj::addVel(int i, btVector3 velMod)
{

    bodies[i].body->setLinearVelocity(
        bodies[i].body->getLinearVelocity()+velMod
    );
}

FIVector4* BaseObj::getVel(int i)
{

    if(i<bodies.size())
    {

        linVelocity=convertToVQV(bodies[i].body->getLinearVelocity());
    }


    return &linVelocity;
}

BodyStruct* BaseObj::getBodyByBoneId(int id)
{
    int i;

    for(i=0; i<bodies.size(); i++)
    {
        if(bodies[i].boneId==id)
        {
            return &(bodies[i]);
        }
    }

    return NULL;
}

float BaseObj::getTotalMass()
{
    int i;

    float tot=0.0f;

    for(i=0; i<bodies.size(); i++)
    {
        tot+=bodies[i].mass;
    }

    return tot;
}

float BaseObj::getMarkerMass()
{
    return bodies[E_BDG_CENTER].mass;
}



void BaseObj::wakeAll()
{
    // int i;

    // for (i = 0; i < bodies.size(); i++) {
    // 	bodies[i].body->setActivationState(ACTIVE_TAG);
    // }
}



bool BaseObj::allFalling()
{
    int i;

    for(i=0; i<bodies.size(); i++)
    {
        if(bodies[i].hasContact)
        {
            return false;
        }
    }

    return true;
}

bool BaseObj::baseContact()
{
    return bodies[E_BDG_CENTER].hasContact;
}

float BaseObj::getPlanarVel()
{

    if(bodies.size()<1)
    {
        return 0.0f;
    }

    btVector3 sourceVel=bodies[E_BDG_CENTER].body->getLinearVelocity();

    return sqrt(sourceVel.getX()*sourceVel.getX()+sourceVel.getY()*sourceVel.getY());
}

void BaseObj::setLinVel(btVector3 newVel, int i)
{
    if(i<bodies.size())
    {
        bodies[i].body->setLinearVelocity(newVel);
        bodies[i].body->setActivationState(ACTIVE_TAG);
    }
}

void BaseObj::applyImpulses(float timeDelta, int i)
{

    if(i<bodies.size())
    {
        if(bodies[i].totAV.isZero()&&bodies[i].totLV.isZero())
        {

        }
        else
        {

            // if (zeroZ) {
            // 	bodies[i].body->setLinearVelocity(btVector3(
            // 		bodies[i].body->getLinearVelocity().getX(),
            // 		bodies[i].body->getLinearVelocity().getY(),
            // 		0.0f	
            // 	));
            // }

            bodies[i].body->setAngularVelocity(bodies[i].body->getAngularVelocity()+bodies[i].totAV*timeDelta);
            bodies[i].body->applyCentralImpulse(bodies[i].totLV*timeDelta);
            bodies[i].body->setActivationState(ACTIVE_TAG);

        }
    }

}



// void begSwing() {
// 	targWeaponStack.push_back(E_WEAPON_STATE_BEG);
// }
// void endSwing() {
// 	targWeaponStack.push_back(E_WEAPON_STATE_END);
// 	targWeaponStack.push_back(E_WEAPON_STATE_IDLE);
// 	//targWeaponStack.push_back(E_WEAPON_POS_RELAXED);
// }

// int getStackElem(int n) {
// 	std::list<int>::iterator ptr;
// 	int i;

// 	for( i = 0 , ptr = targWeaponStack.begin() ; i < n && ptr != targWeaponStack.end() ; i++ , ptr++ );

// 	if( ptr == targWeaponStack.end() ) {
// 	    // list too short 
// 	    return -1;
// 	}
// 	else {
// 	    // 'ptr' points to n-th element of list
// 	    return *ptr;
// 	}
// }


btVector3 BaseObj::getWeaponPos(int curPos)
{

    float newLR=0.0f;
    float newUD=0.0f;

    switch(curPos)
    {
    case E_WEAPON_POS_RELAXED:
        newLR=0.5f;
        newUD=0.25f;
        break;
    case E_WEAPON_POS_LEFT:
        newLR=0.0f;
        newUD=0.5f;
        break;
    case E_WEAPON_POS_RIGHT:
        newLR=1.0f;
        newUD=0.5f;
        break;
    case E_WEAPON_POS_UP:
        newLR=0.5f;
        newUD=0.0f;
        break;
    case E_WEAPON_POS_DOWN:
        newLR=0.5f;
        newUD=1.0f;
        break;

    case E_WEAPON_POS_UP_LEFT:
        newLR=0.0f;
        newUD=0.25f;
        break;
    case E_WEAPON_POS_UP_RIGHT:
        newLR=1.0f;
        newUD=0.25f;
        break;
    case E_WEAPON_POS_DOWN_LEFT:
        newLR=0.0f;
        newUD=0.75f;
        break;
    case E_WEAPON_POS_DOWN_RIGHT:
        newLR=1.0f;
        newUD=0.75f;
        break;
    case E_WEAPON_POS_FORWARD:
        newLR=0.5f;
        newUD=0.5f;
        break;

    }

    return btVector3(newLR, newUD, 0.0f);
}

void BaseObj::setGrabbedBy(int newId, int handNum)
{
    int i;

    isGrabbedByHand=handNum;
    isGrabbedById=newId;

    int heldUID=newId;

    if(heldUID<0)
    {
        heldUID=-3;
    }

    for(i=0; i<bodies.size(); i++)
    {
        assert(false);
//        bodies[i].body->heldByUID=heldUID;
    }
}

// void updateWeaponTargs(double curStepTime) {


// 	totWeaponTime += curStepTime;


// 	if (totWeaponTime >= 1.0) {
// 		totWeaponTime = 1.0;

// 		if (targWeaponStack.size() > 1) {
// 			//targWeaponStack.pop_front();
// 			targWeaponStack.erase(targWeaponStack.begin() + 0);
// 			totWeaponTime = 0.0f;
// 		}
// 	}

// 	int curStep = E_WEAPON_STATE_IDLE;
// 	int nextStep = E_WEAPON_STATE_IDLE;

// 	if (targWeaponStack.size() > 0) {
// 		curStep = targWeaponStack[0];
// 	}

// 	if (targWeaponStack.size() > 1) {
// 		nextStep = targWeaponStack[1];
// 	}
// 	else {
// 		nextStep = curStep;
// 	}






// 	float lerpTime = totWeaponTime;

// 	btVector3 res0;
// 	btVector3 res1;


// 	if (
// 		(curStep == E_WEAPON_STATE_IDLE) &&
// 		(nextStep == E_WEAPON_STATE_IDLE)	
// 	) {
// 		res0 = getWeaponPos(E_WEAPON_POS_RELAXED);
// 		res1 = getWeaponPos(E_WEAPON_POS_RELAXED);
// 	}

// 	if (
// 		(curStep == E_WEAPON_STATE_BEG) &&
// 		(nextStep == E_WEAPON_STATE_BEG)
// 	) {
// 		res0 = getWeaponPos(E_WEAPON_POS_UP_LEFT);
// 		res1 = getWeaponPos(E_WEAPON_POS_UP_LEFT);
// 	}

// 	if (
// 		(curStep == E_WEAPON_STATE_IDLE) &&
// 		(nextStep == E_WEAPON_STATE_BEG)	
// 	) {
// 		res0 = getWeaponPos(E_WEAPON_POS_RELAXED);
// 		res1 = getWeaponPos(E_WEAPON_POS_UP_LEFT);
// 	}


// 	if (
// 		(curStep == E_WEAPON_STATE_BEG) &&
// 		(nextStep == E_WEAPON_STATE_END)	
// 	) {
// 		if (totWeaponTime < 0.5f) {
// 			res0 = getWeaponPos(E_WEAPON_POS_UP_LEFT);
// 			res1 = getWeaponPos(E_WEAPON_POS_FORWARD);
// 			lerpTime = totWeaponTime*2.0f;
// 		}
// 		else {
// 			res0 = getWeaponPos(E_WEAPON_POS_FORWARD);
// 			res1 = getWeaponPos(E_WEAPON_POS_RIGHT);
// 			lerpTime = (totWeaponTime-0.5f)*2.0f;
// 		}
// 	}

// 	if (
// 		(curStep == E_WEAPON_STATE_END) &&
// 		(nextStep == E_WEAPON_STATE_IDLE)	
// 	) {
// 		res0 = getWeaponPos(E_WEAPON_POS_RELAXED);
// 		res1 = getWeaponPos(E_WEAPON_POS_RIGHT);
// 	}



// 	lrBounds = mixf(res0.getX(), res1.getX(), lerpTime);
// 	udBounds = mixf(res0.getY(), res1.getY(), lerpTime);


// }


// void updateWeapon(
// 	int handNum,
// 	btVector3 weaponBeg,
// 	btVector3 weaponEnd,
// 	double curStepTime,
// 	// float lrBounds,
// 	// float udBounds,
// 	float weaponLen
// ) {

// 	totTime += curStepTime;

// 	//updateWeaponTargs(curStepTime);




// 	float myMat[16];
// 	Matrix4 myMatrix4;
// 	Vector3 myVector0;
// 	Vector3 myVector1;
// 	Vector3 normVec;
// 	Vector4 resVector0;
// 	Vector4 resVector1;

// 	Vector4 vf0;
// 	Vector4 vf1;

// 	btVector3 basePos;
// 	float rad0 = 1.0f;
// 	float rad1 = rad0 + weaponLen;

// 	//float lrBounds = sin(totTime/4.0);
// 	//float udBounds = sin(totTime);
// 	//float udBounds2 = udBounds;//sin(totTime/8.0);

// 	if (bodies.size() < 1) {
// 		return;
// 	}


// 	// float weaponTheta = M_PI_2 + lrBounds*M_PI_8;
// 	// float weaponPhi = M_PI_4 + udBounds*M_PI_4;

// 	// float weaponTheta2 = (1.0f - lrBounds)*M_PI + cos(totTime/2.0f)*0.1f;
// 	// float weaponPhi2 = 0 + udBounds*M_PI_2*1.5f + sin(totTime/3.0f)*0.1f;


// 	bodies[E_BDG_CENTER].body->getWorldTransform().getOpenGLMatrix(myMat);
// 	myMatrix4 = Matrix4(myMat);

// 	// myVector0 = Vector3(
// 	// 	cos(weaponTheta)*sin(weaponPhi)*rad0,
// 	// 	sin(weaponTheta)*sin(weaponPhi)*rad0 + 0.5f,
// 	// 	cos(weaponPhi)*rad0 + (1.0f-udBounds2)*0.75f
// 	// );
// 	// myVector1 = Vector3(
// 	// 	cos(weaponTheta2)*sin(weaponPhi2)*rad1,
// 	// 	sin(weaponTheta2)*sin(weaponPhi2)*rad1 + 0.5f,
// 	// 	cos(weaponPhi2)*rad1
// 	// );

// 	// myVector0.x -= (myVector0.x*0.5f + myVector1.x*0.5f)*0.25f;
// 	// myVector0.y -= (myVector0.y*0.5f + myVector0.y*0.5f)*0.25f;

// 	// myVector0 *= 0.75f;

// 	// myVector0.y += 0.25f;

// 	// //if (myVector1.x > 0.0f) {
// 	// 	myVector0.x += myVector1.x*0.25f;
// 	// //}

// 	// myVector1.y += 1.0f-abs(cos(weaponPhi2));

// 	BodyStruct* handBody;

// 	if (handNum == RLBN_LEFT) {
// 		handBody = getBodyByBoneId(getCorrectedName(E_BONE_L_METACARPALS));
// 	}
// 	else {
// 		handBody = getBodyByBoneId(getCorrectedName(E_BONE_R_METACARPALS));
// 	}



// 	btVector3 handCenter = handBody->body->getCenterOfMassPosition();

// 	myVector0 = Vector3(weaponBeg.getX(), weaponBeg.getY(),weaponBeg.getZ());
// 	myVector1 = Vector3(weaponEnd.getX(), weaponEnd.getY(),weaponEnd.getZ());


// 	normVec = myVector1 - myVector0;
// 	normVec.normalize();
// 	normVec = normVec*(rad1-rad0);
// 	myVector1 = myVector0 + normVec;

// 	rightHandTop = true;//(myVector0.x < 0.0f);



// 	vf0 = Vector4(myVector0.x, myVector0.y, myVector0.z, 1.0f);
// 	vf1 = Vector4(myVector1.x, myVector1.y, myVector1.z, 1.0f);

// 	resVector0 = myMatrix4*vf0;
// 	resVector1 = myMatrix4*vf1;

// 	weaponVec0[handNum] = btVector3(resVector0.x,resVector0.y,resVector0.z);
// 	weaponVec1[handNum] = btVector3(resVector1.x,resVector1.y,resVector1.z);

// 	btVector3 weapDif = handCenter-weaponVec0[handNum];

// 	weaponVec0[handNum] += weapDif;
// 	weaponVec1[handNum] += weapDif;


// 	vf0 = Vector4( 1.0f,0.0f,0.0f,1.0f);
// 	vf1 = Vector4(-1.0f,0.0f,0.0f,1.0f);

// 	resVector0 = myMatrix4*vf0;
// 	resVector1 = myMatrix4*vf1;

// 	rightVec = btVector3(resVector0.x,resVector0.y,resVector0.z);
// 	leftVec = btVector3(resVector1.x,resVector1.y,resVector1.z);






// }


void BaseObj::flushImpulses()
{

    int i;

    for(i=0; i<bodies.size(); i++)
    {
        bodies[i].totAV=btVector3(0.0f, 0.0f, 0.0f);
        bodies[i].totLV=btVector3(0.0f, 0.0f, 0.0f);
    }


}

float BaseObj::turnTowardsTargAng(float targAng)
{
    btVector3 curVec=bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis()*btVector3(0.0f, 1.0f, 0.0f);
    curVec.setZ(0.0f);
    curVec.normalize();

    float curAng=atan2(curVec.getY(), curVec.getX());

    return getShortestAngle(curAng, targAng, 1.0f);
}

float BaseObj::turnTowardsPointDelta(btVector3 targPoint)
{
    btVector3 centerPoint=getCenterPoint(E_BDG_CENTER);

    btVector3 targVec=targPoint-centerPoint;
    targVec.setZ(0.0f);
    targVec.normalize();

    float targAng=atan2(targVec.getY(), targVec.getX());

    return turnTowardsTargAng(targAng);


    //return targAng-curAng;

    // btQuaternion quat;

    // if ( abs(vectorA.dot(vectorB)) == 1.0f ) {
    // 	// todo: handle (anti)parallel case
    // 	//curJoint->pivotAxis = btVector3(0.0f,0.0f,0.0f);
    // 	//curJoint->quat = btQuaternion(btVector3(0.0f,0.0f,1.0f), 0.0f);
    // }
    // else {
    // 	axis = (vectorA.cross(vectorB)).normalized();
    // 	angle = btAcos(vectorA.dot(vectorB)) / (vectorA.length() * vectorB.length());
    // 	quat = btQuaternion(axis, angle);
    // 	transform.setRotation(curJoint->quat);
    // 	//curJoint->pivotAxis = axis;
    // }
}

void BaseObj::applyAngularImpulse(btVector3 newAV, bool delayed, int i)
{


    if(i<bodies.size())
    {
        if(delayed)
        {
            bodies[i].totAV+=newAV;
        }
        else
        {

            bodies[i].body->setAngularVelocity(bodies[i].body->getAngularVelocity()+newAV);
            bodies[i].body->setActivationState(ACTIVE_TAG);
        }

    }

}

void BaseObj::applyImpulse(btVector3 imp, bool delayed, int i)
{
    if(i<bodies.size())
    {

        if(delayed)
        {
            bodies[i].totLV+=imp;
        }
        else
        {


            bodies[i].body->applyCentralImpulse(imp);
            bodies[i].body->setActivationState(ACTIVE_TAG);
        }
    }

}

void BaseObj::applyImpulseOtherRot(btVector3 imp, btMatrix3x3 otherRot, bool delayed, int i)
{

    //Vector3 myRHS = Vector3(imp.getX(),imp.getY(),imp.getZ());
    //Vector3 res = otherRot*myRHS;
    btVector3 newImp=otherRot*imp;//btVector3(res.x,res.y,res.z);

    if(i<bodies.size())
    {
        if(delayed)
        {
            bodies[i].totLV+=newImp;
        }
        else
        {

            bodies[i].body->applyCentralImpulse(newImp);
            bodies[i].body->setActivationState(ACTIVE_TAG);
        }
    }


}

bool BaseObj::isHumanoid()
{
    return (
        (entType==E_ENTTYPE_NPC)
        );
}

// void makeWalk(btVector3 imp, btMatrix3x3 otherRot) {

// 	if (isHumanoid()) {

// 	}
// 	else {
// 		return;
// 	}


// 	bool lfDown = bodies[E_BDG_LFOOT].hasContact && 
// 		(bodies[E_BDG_LFOOT].body->getLinearVelocity().length() < 0.1f);
// 	bool rfDown = bodies[E_BDG_RFOOT].hasContact && 
// 		(bodies[E_BDG_RFOOT].body->getLinearVelocity().length() < 0.1f);

// 	int resInd = -1;

// 	if (lfDown && rfDown) {
// 		leftActive = !leftActive;
// 		//resInd = E_BDG_LFOOT;

// 		if (leftActive) {
// 			resInd = E_BDG_LFOOT;
// 		}
// 		else {
// 			resInd = E_BDG_RFOOT;
// 		}

// 	}
// 	// else {
// 	// 	if (lfDown) {
// 	// 		resInd = E_BDG_LFOOT;
// 	// 	}
// 	// 	else {
// 	// 		if (rfDown) {
// 	// 			resInd = E_BDG_RFOOT;
// 	// 		}
// 	// 	}
// 	// }

// 	if (resInd == -1) {
// 		return;
// 	}


// 	btVector3 newImp = otherRot*imp*bodies[resInd].mass;

// 	bodies[resInd].body->applyCentralImpulse(newImp);
// 	bodies[resInd].body->setActivationState(ACTIVE_TAG);


// }



void BaseObj::moveOffset(btVector3 offset, int ind)
{
    btTransform trans;


    if(ind<bodies.size())
    {

        trans.setIdentity();
        trans.setOrigin(
            bodies[ind].body->getCenterOfMassPosition()+offset
        );
        bodies[ind].body->setActivationState(ACTIVE_TAG);
        bodies[ind].body->setCenterOfMassTransform(
            trans
        );

    }
}

void BaseObj::moveToPoint(btVector3 newPoint, int ind)
{
    btTransform trans;


    if(ind<bodies.size())
    {

        trans.setIdentity();
        trans.setOrigin(newPoint);
        bodies[ind].body->setActivationState(ACTIVE_TAG);
        bodies[ind].body->setCenterOfMassTransform(
            trans
        );

    }
}

// void setCenterPoint(FIVector4* newPos) {

// 	centerPoint.copyFrom(newPos);

// }

btVector3 BaseObj::getCenterPoint(int ind)
{
    if(
        ind<bodies.size()
        )
    {
        return bodies[ind].body->getCenterOfMassPosition();
    }
    else
    {
        return btVector3(0.0f, 0.0f, 0.0f);
    }
}

FIVector4* BaseObj::getCenterPointFIV(int ind)
{
    if(
        ind<bodies.size()
        )
    {
        centerPoint=convertToVQV(bodies[ind].body->getCenterOfMassPosition());
    }
    else
    {
        centerPoint.setFXYZ(0.0f, 0.0f, 0.0f);
    }

    return &centerPoint;
}


BaseObj::BaseObj()
{

}

void BaseObj::removeChild(BaseObjType _uid)
{
    int i;

    for(i=0; i<children.size(); i++)
    {
        if(children[i]==_uid)
        {
            children.erase(children.begin()+i);
            return;
        }
    }
}


float BaseObj::healthPerc()
{
    return ((float)statSheet.curStatus[E_STATUS_HEALTH])/((float)statSheet.maxStatus[E_STATUS_HEALTH]);
}

bool BaseObj::isDead()
{
    return (statSheet.curStatus[E_STATUS_HEALTH]<=0);
}

bool BaseObj::isAlive()
{
    return (statSheet.curStatus[E_STATUS_HEALTH]>0);
}

bool BaseObj::getActionState(int action, int handNum)
{
    return actionStates[action*RLBN_LENGTH+handNum];
}

void BaseObj::setActionState(int action, int handNum, bool newVal)
{
    actionStates[action*RLBN_LENGTH+handNum]=newVal;
}

void BaseObj::clearActionStates()
{
    int i;

    for(i=0; i<E_ACT_LENGTH*RLBN_LENGTH; i++)
    {
        actionStates[i]=false;
    }

}


void BaseObj::init(
    BaseObjType _uid,
    BaseObjType _parentUID,
    int _objectType,
    int _entType,
    int _subType,
    FIVector4* cellPos
)
{

    int i;

    contactCount=0;

    //mass = 10.0f;
    orgId=-1;
    actorId=-1;
    isHidden=false;

    maxFrames=0;
    objectType=_objectType;
    entType=_entType;
    subType=_subType;

    behaviorTarget=btVector3(0.0f, 0.0f, 0.0f);
    npcRepel=btVector3(0.0f, 0.0f, 0.0f);

    isGrabbedByHand=-1;
    isGrabbedById=-1;

    lastBlockDis=0.0f;
    blockCount=0.0f;
    swingCount=0.0f;
    bindingPower=1.0f;
    airCount=0.0f;

    tbDir=0;

    for(i=0; i<RLBN_LENGTH; i++)
    {
        isGrabbingId[i]=-1;
        swingType[i]=E_PG_SLSH;
    }

    for(i=0; i<E_CS_LENGTH; i++)
    {
        statSheet.baseStats[i]=5;
        statSheet.unapplyedStats[i]=5;
    }
    statSheet.availPoints=10;

    for(i=0; i<E_STATUS_LENGTH; i++)
    {
        statSheet.curStatus[i]=10;
        statSheet.maxStatus[i]=10;
    }


    zeroZ=false;
    jumpCooldown=0;
    hitCooldown=0;


    clearActionStates();


    isOpen=false;
    isEquipped=false;
    parentUID=_parentUID;
    uid=_uid;

    skelOffset=btVector3(0.0f, 0.0f, 0.0f);
    startPoint=convertToBTV(*cellPos);

    bounciness=0.0f;
    friction=0.9f;
    windResistance=0.9f;


    //tbPos = getUnitBounds(false);

    targPath.points[0]=btVector3(0.0f, 0.0f, 0.0f);
    targPath.points[1]=btVector3(0.0f, 0.0f, 0.0f);
    targPath.searchedForPath=false;
    targPath.didFindPath=false;
    targPath.finalPoints.clear();
    targPath.nextInd=-1;


}

