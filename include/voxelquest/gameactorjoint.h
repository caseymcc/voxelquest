#ifndef _voxelquest_gameactorjoint_h_
#define _voxelquest_gameactorjoint_h_


class GameActorJoint
{
public:
    GameActorJoint()
    {

    }

    void init()
    {

    }

private:
    btDynamicsWorld* m_ownerWorld;
    int uid;

    //btDynamicsWorld* ownerWorld, const btVector3& positionOffset, bool bFixed, int _uid	
    // m_ownerWorld = ownerWorld;
    // uid = _uid;

    GameActorJoint* parentJoint;
    std::vector<GameActorJoint*> childrenJoints;

    btCollisionShape* ajShape;
    btRigidBody* ajBody;
    btTypedConstraint* ajJoint;
};


#endif//_voxelquest__h_